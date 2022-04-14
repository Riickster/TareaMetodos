//Actividad Integradora: Resaltador de Sintaxis
//El siguiente codigo incluye un resaltado de Sintaxis para el lenguaje de programacion C++
//Hecho por:
//Jose Miguel Beltrán Cinco A00227714
//Alejandro Hernández A01721287
//Ricardo Garza A01197406
//Octavio Augusto Alemán Esparza A01660702
//Erick Daniel Padilla Verdugo A01740287
//Para la Materia de Implementacion de Metodos computacionales.
#include <iostream>
#include <string>
#include <unordered_map>
#include <regex>
#include <fstream>
#include <ctype.h>

using namespace std;

ifstream lecturaR("debuging.txt", ios::binary);       //Archivo a leer
ofstream escritura("index.html", ios::binary);       //Archivo Construido

unordered_map<std::string, std::string> umap = {        //UMAP para guardar las palabras reservadas
        {"program","tk_program"},
        {"begin","tk_begin"},
        {"read","tk_read"},
        {"var","tk_var"},
        {"int","tk_int"},
        {"integer","tk_int"},
        {"float","tk_float"},
        {"real","tk_real"},
        {"end","tk_end"},
        {"write","tk_write"},
        {"print","tk_print"},
        {"pause","tk_pause"},
        {"none","tk_none"}
};

//Funcion para validar identificadores
bool isID(string split){
    const regex expReg("[ ]*[a-zA-Z]\\w*");
    //La expresin \\w significa cualquier caracter (palabra) incluye mayusculas, minusculas, digitos y guion bajo
    return regex_match(split, expReg);
}
//Funcion para validad palabras reservadas
//En la implementacion solo se buscar la palabra en el UMAP
bool isKeyword(string split){
    for(int i = 0; i < split.size(); i++) {
        if(split[i] != ' ') {
            split = split.substr(i, split.size() - i);
            break;
        }
    }
    auto result = umap.find(split);
    if(result != umap.end()) return 1;
    else return 0;
}

bool isOperand(string split, int operandsSize){
    string operands[6] = {";", "+", "=", ":", ",","*"};
    for(int i = 0; i < operandsSize; i++){
        if(operands[i] == split){
            return true;
        } 
    }
    return false;
}

//Funcion para validar constantes
//Documento tambien como diferenciar los diferentes tipos
bool isConst(string split) {
    const regex expReg("[ ]*(|-)[0-9]+(|[.][0-9]+(|((e|E)(|-)[0-9]+)))");   //Cualquier Constante
    /* Esta Regex Indica la busqueda de Entero Positivo, Entero negativo, Flotante con o sin signo y flotante con exponente con o sin signo
    */
    return regex_match(split, expReg);
}

//Reconoce agrupa las funciones de validacion, tomando con prioridad la de palabras reservadas
void reconoce(string& sp, bool& dif_esp) {
    if(isKeyword(sp)){
        escritura << "\t\t<span class=\"Keyword\">" << sp << "</span>" << endl;
    }   
    else if(isID(sp)){
        escritura << "\t\t<span class=\"id\">" << sp << "</span>" << endl;
    }
    else if(isConst(sp)){
        escritura << "\t\t<span class=\"const\">" << sp << "</span>" << endl;
    }
    else if(isOperand(sp, 6)){
        escritura << "\t\t<span class=\"operand\">" << sp << "</span>" << endl;
    }
    else{
        if(sp != ""){
            escritura << "\t\t<span class=\"syntaxError\">" << sp << "</span>" << endl;
        }
    }
    sp = "";
    dif_esp = false;
}
template <typename T> std::string type_name();
//Escribe el inicio del HTML
void iniciohtml() {
    escritura <<
        "<!DOCTYPE html>\n" <<
        "<html>\n" <<
        "    <head>\n" <<
        "        <meta charset=\"utf-8\"/>\n" <<
        "        <title>Resaltador de Sintaxix</title>\n" <<
        "        <link rel=\"stylesheet\" href=\"estilos.css\">\n" <<
        "    </head>\n" <<
        "    <body>\n";
}
//Escribe el final del HTML
void finalhtml() {
    escritura <<
        "    </body>\n" <<
        "</html>\n";
}

int main(){
    char operands[6] = {';', '+', '=', ':', ',','*'};
    string linea, lineaR;
    iniciohtml();
    ofstream spacing("debuging_temp.txt");
    bool hasSpacing = 0;
    while(getline(lecturaR, lineaR)){
        for(int j = 0; j < linea.size(); j++){
            if(linea[j] == ' '){
                hasSpacing = 1;
            }
        }
        if(hasSpacing == 0 && lineaR.size() > 2){
            for(int i = 0; i < 6; i++){
                for(int j = 0; j < lineaR.size(); j++){
                    if(operands[i] == lineaR[j]){
                        if(operands[i] == ';'){
                            lineaR.insert(j, " ");
                        } else {
                            lineaR.insert(j+1, " ");
                            lineaR.insert(j, " ");
                        }
                        break;
                    }
                }
            }
        }
        spacing << lineaR << endl;
        hasSpacing = 0;
    }

    //Se abren cada una de las lineas en el documento de lectura
    ifstream lectura("debuging_temp.txt", ios::binary);       //Archivo a leer
    while(getline(lectura, linea)){
        string T;
        bool isProgram = false, dif_esp = 0, isExcecuted = 0;
        stringstream X(linea); 
        while(getline(X, T, ' ')){
            bool isSpace = 0;
            for(int i = 0; i < 6; i++){
                if(T.find(operands[i]) != string::npos){
                    T.erase(T.find(operands[i]), 1);
                    for(int i=0; i<T.size(); i++){
                        if(isspace(T[i])){
                            T.erase(i, 1);
                        }
                    }
                    if(isExcecuted == 0){
                        reconoce(T, dif_esp);
                        T = operands[i];
                        reconoce(T, dif_esp);
                    }
                }
            }
            if(isExcecuted == 0){
                reconoce(T, dif_esp);
            }
            isSpace = 0;
        }
        escritura << "\t\t<br>" << endl;        //Tras terminar de leer la linea, genera salto de linea
    }
    finalhtml();
    return 0;
}
