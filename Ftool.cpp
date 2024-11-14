#include<iostream>
#include<fstream>
#include<Windows.h>
#include<cstdlib>
#include<io.h>
#include<stdexcept>

std::string get_executable_path()
{
    char path[MAX_PATH];
    if (GetModuleFileNameA(NULL, path, MAX_PATH) == 0) {
        throw std::logic_error("Failed to get executable path.");
    }
    return std::string(path);
}

std::string get_executable_directory()
{
    std::string exe_path = get_executable_path();
    size_t pos = exe_path.find_last_of("\\/");
    return exe_path.substr(0, pos); 
}

const std::string TEMPLATE_PATH = get_executable_directory() + "\\template_path.txt";

void save_template_path(const std::string &path) 
{
    std::ofstream out_file(TEMPLATE_PATH);
    if(out_file.is_open()){
        out_file << path << std::endl;
        std::cout << "Template path set to: " << path << std::endl;
    }
    else{
        std::cerr << "Failed to save template path to: " << TEMPLATE_PATH << std::endl;
    }
}

std::string read_template_path()
{
    std::ifstream in_file(TEMPLATE_PATH);
    std::string s;
    if(in_file.is_open())
        std::getline(in_file, s);
    return s;
}

std::string generate_cpp_name(int index) {
    std::string result;
    while (index >= 0) {
        result.insert(result.begin(), 'A' + (index % 26));
        index = index / 26 - 1;
    }
    return result;
}

void create_contest(const std::string &name, int n, const std::string &template_path)
{
    std::cout << "Create contest: " << name << std::endl;

    if(!CreateDirectory(name.c_str(), NULL) && GetLastError() != ERROR_ALREADY_EXISTS){
        std::cerr << "Failed to create directory: " << name << std::endl;
        return;
    }

    for(int i = 0; i < n; i ++){
        std::string ch = generate_cpp_name(i);
        std::string cpp_path = name + "\\" + ch + ".cpp";

        std::ofstream out_file(cpp_path);
        if(!out_file.is_open()){
            std::cerr << "Failed to create: " << ch << ".cpp" << std::endl;
            continue;
        }

        if(!template_path.empty() && _access(template_path.c_str(), 0) == 0){
            std::ifstream in_file(template_path);
            if(in_file.is_open()){
                out_file << in_file.rdbuf();
            }
            else{
                std::cerr << "Failed to read template: " << template_path << std::endl;
            }
        }

        std::cout << "Create " << ch << ".cpp" << std::endl;
    }

    std::ofstream in(name + "\\input.in");
    std::ofstream out(name + "\\output.out");

    std::cout << "Done!" << std::endl;
}
void show_help()
{
    std::cout << std::endl;
    std::cout << "Usage: ftool <action>" << std::endl;
    std::cout << "Actions:" << std::endl;
    std::cout << "  template <path>       Setting template path" << std::endl;
    std::cout << "  contest <path> <num>  Create a contest with <num> questions in <path>" << std::endl;
}

int main(int argc, char *argv[])
{
    if(argc < 2){
        std::cerr << "Unknown action" << std::endl;
        std::cerr << "Use \"ftool help\" to get usage" << std::endl;
        return 1;
    }

    std::string op = argv[1];
    std::string template_path = read_template_path();

    if(op == "help"){
        show_help();
    }
    else if(op == "template"){
        if(argc < 3){
            std::cerr << "Missing parameter: " << "template <path>" << std::endl;
            return 1;
        }
        std::string s = argv[2];
        save_template_path(s);
    }
    else if(op == "contest"){
        if(argc < 4){
            std::cerr << "Missing parameter: " << "contest <path> <num>" << std::endl;
            return 1;
        }
        std::string s = argv[2];
        int n = std::stoi(argv[3]);
        create_contest(s, n, template_path);
    }
    else{
        std::cerr << "Unknown action" << std::endl;
        std::cerr << "Use \"ftool help\" to get usage" << std::endl;
        return 1;
    }

    return 0;
}