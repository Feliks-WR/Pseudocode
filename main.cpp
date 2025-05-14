#include ".Includes/includes.h"

#include <sstream>
#include <fstream>
#include <string>
#include <optional>
#include <iostream>
#include <cstring>
#include <unistd.h>

using namespace cstring;
using namespace std;


constexpr czstring COMMON_INCLUDES = "#include \"custom_lib.h\"\n";
constexpr czstring MAIN_START = "\n\nint main() {\n\tstd::cout.setf(std::ios::boolalpha);\n";
constexpr czstring MAIN_END = "\treturn 0;\n}";


#if __cplusplus < 202002L
#error "Transpiler requires C++20 or higher"
#endif // __cplusplus < 202002L


void parseAll(czstring codeFile, czstring outputFile) {
    cout << "Transpiling '" << codeFile << "'...\n";

    ifstream codeReader(codeFile);
    if (!codeReader.is_open()) {
        cerr << "Error: Could not open input file '" << codeFile << "'\n";
        return;
    }

    stringstream intermediate_1, intermediate_2;
    ofstream outputWriter(outputFile, ios::trunc);
    if (!outputWriter.is_open()) {
        cerr << "Error: Could not open output file '" << outputFile << "' for writing\n";
        codeReader.close();
        return;
    }


    // Pass 1: Declutter (remove comments, empty lines)

    string line;
    while (getline(codeReader, line))
    {
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty() || line.starts_with("//")) {
            continue;
        } else {
            size_t comment_pos = line.find("//");
            if (comment_pos != string::npos) {
                line = line.substr(0, comment_pos);
                 line.erase(line.find_last_not_of(" \t\r\n") + 1);
            }
            if (line.empty()) continue;

            intermediate_1 << line << '\n';
        }
    }

    codeReader.close();

    intermediate_1.clear();
    intermediate_1.seekg(0);

    while (getline(intermediate_1, line))
    {
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);

        if (line.empty()) continue;

        line = parseSymbols(line);

        intermediate_2 << line << '\n';
    }

    intermediate_2.clear();
    intermediate_2.seekg(0);

    outputWriter << COMMON_INCLUDES;

    // find the (unmentioned) main function

    size_t pos_end_of_last_module{ string::npos };
    size_t temp { static_cast<size_t>(intermediate_2.tellg()) };

    intermediate_2.clear();
    intermediate_2.seekg(0);

    while (getline(intermediate_2, line)) 
    {
        if (line.starts_with("ENDPROCEDURE") || line.starts_with("ENDFUNCTION")) {
            pos_end_of_last_module = intermediate_2.tellg();
        }
    }

    auto convert_to_cpp = [&](optional<size_t> till, bool to_indent = true) -> void {
        while (intermediate_2.tellg() < till.value_or(static_cast<size_t>(-1)) && getline(intermediate_2, line)) 
        {
            line.erase(0, line.find_first_not_of(" \t\r\n"));
            line.erase(line.find_last_not_of(" \t\r\n") + 1);

            if (line.empty()) continue;

            {
                std::string cleansedLine{ line };

                cleansedLine = replaceOpening(cleansedLine);
                cleansedLine = replaceClosing(cleansedLine);

                if (cleansedLine != line) {
                    outputWriter << cleansedLine;
                    continue;
                }
                // else
                //     line = cleansedLine;
            }

            if (to_indent)
                outputWriter << '\t';

            if (line.starts_with("OUTPUT")) {
                outputWriter << parseOutputString(line);
            } 
            else if (line.starts_with("INPUT")) {
                outputWriter << parseInputString(line);
            }
            else if (line.starts_with("IF")) {
                outputWriter << parseIf(line);
            }
            else if (line.starts_with("ELSE")) {
                outputWriter << parseElse(line);
            }
            else if (line.starts_with("WHILE")) {
                outputWriter << parseWhile(line);
            }
            else if (line.starts_with("REPEAT")) {
                outputWriter << repeatEquivilant();
            }
            else if (line.starts_with("UNTIL")) {
                outputWriter << parseUntil(line);
            }
            else if (line.starts_with("FOR")) {
                outputWriter << parseFor(line);
            }
            else if (line.starts_with("NEXT")) {
                outputWriter << nextEquivalent();
            }
            else if (line.starts_with("CALL")) {
                outputWriter << callEquivalent(line);
            }
            else if (line.starts_with("PROCEDURE")) {
                outputWriter << parseProcedure(line);
            }
            else if (line.starts_with("RETURN"))
                outputWriter << parseReturn(line);
            else if (line.starts_with("TYPE"))
                outputWriter << parseType(line);
            else if (line.starts_with("ENDTYPE"))
                outputWriter << endTypeEquivalent();
            else if (line.starts_with("DECLARE")) {
                try {
                    outputWriter << parseDeclareString(line);
                } 
                catch (const std::runtime_error& e) { 
                    cerr << "Transpilation Error: " << e.what() << " in line: " << line << '\n';
                    outputWriter << "#error Transpilation Error: " << e.what();
                    return;
                } 
                catch (const std::out_of_range& e) {
                    cerr << "Transpilation Error (out of range): " << e.what() << " in line: " << line << '\n';
                    outputWriter << "#error Transpilation Error (out of range): " << e.what();
                    return;
                }

            } else {
                if (!line.ends_with(";") && !line.ends_with("}") && line.find_first_not_of(" \t\r\n") != string::npos) {
                    line += ";";
                }

                outputWriter << line;
            }

            outputWriter << '\n';
        }
    };

    intermediate_2.clear();
    intermediate_2.seekg(0);

    // before main function
    convert_to_cpp(pos_end_of_last_module == string::npos ? 0ul : pos_end_of_last_module, false); 

    outputWriter << MAIN_START;

    intermediate_2.clear();
    intermediate_2.seekg(pos_end_of_last_module == string::npos ? 0ul : pos_end_of_last_module);

    convert_to_cpp(nullopt);       // after main function

    outputWriter << MAIN_END;

    outputWriter.close();

    cout << "Transpilation complete. Output written to '" << outputFile << "'\n";
    clog << "Transpilation complete. Output written to '" << outputFile << "'\n";
}

int main() {
    czstring codeFile = "code.txt";
    czstring outputFile = "output-code.cpp";

    ofstream ofs("logs/" __TIMESTAMP__ ".txt");
    streambuf* oldrdbuf_clog = clog.rdbuf(ofs.rdbuf());
    streambuf* oldrdbuf_cerr = cerr.rdbuf(ofs.rdbuf());

    parseAll(codeFile, outputFile);

    clog.rdbuf(oldrdbuf_clog);
    cerr.rdbuf(oldrdbuf_cerr);

#if defined(__linux__) || defined(__APPLE__)
    std::ignore = std::system("clear");
#elif defined(_WIN32) || defined(_WIN64)
    std::ignore = std::system("cls");
#endif


    return 0;
}