#include <iostream>     // mostrar cosas por pantalla (cout)
#include <vector>       // crear listas y matrices dinámicas
#include <queue>        // usar una cola (para BFS)
#include <algorithm>    // usar swap
#include <ctime>        // usar la hora actual

using namespace std;
// para no escribir todo el tiempo std, pero sirve mas para codigos pequenhos
// Tamaño del laberinto
int filas, columnas; // son variables globales dinamicas
// Guarda cuántas filas y columnas tendrá el laberinto.

// Estructura que representa una celda
struct Celda {
    bool visitada = false;
    bool paredArriba = true;
    bool paredAbajo = true;
    bool paredIzquierda = true;
    bool paredDerecha = true;
};

// GENERAR LABERINTO (DFS y bv)
void generarLaberinto(vector<vector<Celda>> &lab, int f, int c) { // aca uso mi dfs & es como referencia 
    // Crea el laberinto usando DFS con recursividad.
    lab[f][c].visitada = true;  // Busca el camino desde la entrada hasta la salida usando BFS

    int df[4] = {1, -1, 0, 0};   // Cambios de fila: abajo, arriba, derecha, izquierda
    int dc[4] = {0, 0, 1, -1};   // Cambios de fila: abajo, arriba, derecha, izquierda

    // Mezcla simple usando rand()      // recorre las 4 posiciones
    for (int i = 0; i < 4; i++) {
        int r = rand() % 4;    // elige un indice random entre 0 y 3
        swap(df[i], df[r]);    // mezcla el orden de las direcciones
        swap(dc[i], dc[r]);  
    }

    for (int i = 0; i < 4; i++) {   // Intenta moverse en las 4 direcciones (ya mezcladas)
        int nf = f + df[i]; // nueva fila
        int nc = c + dc[i]; // nueva columna 

        if (nf >= 0 && nf < filas && nc >= 0 && nc < columnas && // para que no salga de la pared && es y para como un and 
            !lab[nf][nc].visitada) {

            // Quitar paredes permite que el algoritmo cree pasillos y construya el laberinto.
            // Quitar paredes
            if (df[i] == 1) { // moverse abajo
                lab[f][c].paredAbajo = false; // quito la pared abajo de la celda actual
                lab[nf][nc].paredArriba = false; // la pared arriba de la celda vecina
            }
            if (df[i] == -1) { // arriba
                lab[f][c].paredArriba = false;
                lab[nf][nc].paredAbajo = false;
            }
            if (dc[i] == 1) { //derecha
                lab[f][c].paredDerecha = false;
                lab[nf][nc].paredIzquierda = false;
            }
            if (dc[i] == -1) { // izquierda
                lab[f][c].paredIzquierda = false;
                lab[nf][nc].paredDerecha = false;
            }

            generarLaberinto(lab, nf, nc); // recursividad, backstracnking 
        }
    }
}
// bfs
// MOSTRAR LABERINTO const para que no se pueda modificar
void mostrarLaberinto(const vector<vector<Celda>> &lab,     // Dibuja el laberinto en la consola usando símbolos:
                    const vector<vector<bool>> &camino) {

    for (int i = 0; i < filas; i++) {

        for (int j = 0; j < columnas; j++) {    //  // Recorre columnas
            cout << "+";    // // Esquina de celda
            cout << (lab[i][j].paredArriba ? "---" : "   ");    // Si hay pared arriba, dibuja --- si no, espacios
        }
        cout << "+\n";    // Cierra la fila superior y salta de línea

        for (int j = 0; j < columnas; j++) {
            cout << (lab[i][j].paredIzquierda ? "|" : " "); // Si hay pared izq, dibuja |, si no, espacio

            if (i == 0 && j == 0)
                cout << " E ";
            else if (i == filas-1 && j == columnas-1)
                cout << " S ";
            else if (camino[i][j])
                cout << " * ";   // Si es parte del camino, marca *
            else
                cout << "   ";   // Dibuja borde derecho final (siempre) y salto de línea
        }
        cout << "|\n";
    }

    for (int j = 0; j < columnas; j++)
        cout << "+---";
    cout << "+\n";
}

// RESOLVER LABERINTO (BFS)
vector<vector<bool>> resolverLaberinto(const vector<vector<Celda>> &lab) { // uso mi bfs 
//Busca el camino desde la entrada hasta la salida usando BFS
    vector<vector<bool>> visitado(filas, vector<bool>(columnas, false));
    vector<vector<pair<int,int>>> padre( // pair se usa para guardar 2 valores juntos 
        filas, vector<pair<int,int>>(columnas, {-1,-1})
    );

    queue<pair<int,int>> cola; // se crea una cola llamada cola
    cola.push({0,0}); // push es para agregar. agrego el elemento 0,0 
    visitado[0][0] = true;

    int df[4] = {1, -1, 0, 0};
    int dc[4] = {0, 0, 1, -1};

    while (!cola.empty()) { // Tomar la siguiente celda pendiente del BFS y procesarla empty verfica si la cola esta vacia 
        auto act = cola.front(); cola.pop(); // auto deduce el tipo de dato, front ver el primer elemento de cola pop elimina el elemento 
        int f = act.first, c = act.second; // extrae los valores pair de fila y col

        if (f == filas-1 && c == columnas-1) // si se lleog a la salida 
            break;

        for (int i = 0; i < 4; i++) { // probar las 4 direcciones 
            int nf = f + df[i]; // calcula la celda vacia 
            int nc = c + dc[i];

            if (nf < 0 || nf >= filas || nc < 0 || nc >= columnas) continue; // verifica los limites del laberinto 
            if (visitado[    nf][nc]) continue; // si ya fue visitada 

            if (df[i] == 1 && lab[f][c].paredAbajo) continue; // && sirve para que tdas las condiciones se cumplan al mismo tiempo
            if (df[i] == -1 && lab[f][c].paredArriba) continue;
            if (dc[i] == 1 && lab[f][c].paredDerecha) continue;
            if (dc[i] == -1 && lab[f][c].paredIzquierda) continue;

            visitado[nf][nc] = true; // se marca la nueva celda visitada
            padre[nf][nc] = {f,c}; // se guarda el padre, cada celda recuerda de que celda vino
            cola.push({nf,nc}); // push agregar entra al final de la cola, se mete la nueva celda a la cola 
        }
    }

    // Reconstruye la ruta desde la salida hasta la entrada usando el mapa del padres
    vector<vector<bool>> camino(filas, vector<bool>(columnas, false));
    int f = filas-1, c = columnas-1; // Está guardando la posición de la salida del laberinto

    while (f != -1 && c != -1) { // bucle que se repite mientras no llegues al valor -1 
        camino[f][c] = true; // && sirve para que tdas las condiciones se cumplan al mismo tiempo
        auto p = padre[f][c]; //  auto deduce el tipo de dato
        f = p.first; // es es primer valor del pair
        c = p.second; // segun valor 
    }

    return camino;
}

// MAIN
int main() {        // srand genera numeros aleatorios 
    srand(time(0)); // genera numeros aleatorios, genera números diferentes cada vez que ejecutás el programa
                    // reinicia mis numeros randoms en cada ejecucion 
    filas = 7;
    columnas = 7;

        vector<vector<Celda>> laberinto(filas, vector<Celda>(columnas)); // Crea el laberinto vacío en memoria.

        generarLaberinto(laberinto, 0, 0); // // Construye el laberinto rompiendo paredes

        vector<vector<bool>> camino = resolverLaberinto(laberinto); // Busca el camino correcto desde la entrada hasta la salida.   

        mostrarLaberinto(laberinto, camino); // dibuja el laberinto 

    return 0;
}
