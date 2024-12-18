#include<raylib.h>
#include<iostream>
#include<vector>
#include<ctime>
#include<stack>

using namespace std;


const int screenWidth = 800;
const int screenHeight = 800;

int cellSize;

class Labyrinthe{
    public:
 
struct Cell{//pour gerer les murs
    int x,y;
    bool visited = false;
    bool topWall = true,rightWall= true, bottomWall =true, leftWall=true;
};

  int rows, cols,cellSize;
    vector<vector<Cell>> grille;
    //constructeur pour initialiser le lab avec les dimensions 
    Labyrinthe(int r, int c) : rows(r), cols(c) {
        cellSize = screenWidth / cols;
        grille.resize(rows, vector<Cell>(cols));
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                grille[i][j] = {j, i};
            }
        }
    }



// methode pour générer le labyrinthe avec l'algorithme de backtracking (DFS)/creation des chemins
void GenererLabyrinthe() {
    srand(time(nullptr));
    stack<Cell*> stack;
    Cell* current = &grille[0][0];
    current->visited = true;
    stack.push(current);

    while (!stack.empty()) {
        current = stack.top();
        int x = current->x;
        int y = current->y;
//liste des voisins non visités est créée. Cette liste stocke les adresses des cellules adjacentes à la cellule courante qui n'ont pas encore été explorées .
//Le but est de rassembler les voisins qui peuvent être explorés ensuite.
        vector<Cell*> voisins;

 // Vérifier les voisins
        if (x > 0 && !grille[y][x - 1].visited) voisins.push_back(&grille[y][x - 1]); // Gauche /on verifie qu on nest pas dans la premiere colonne-> pas de cellule a gauche
        // si la cellule voisine a gaiche est valide on l ajoute a la liste des voisins sous ptr
        if (x < cols - 1 && !grille[y][x + 1].visited) voisins.push_back(&grille[y][x + 1]); // Droite / on verifie au on nest pas dans la derniere colonne->pas de cellule a droite
        if (y > 0 && !grille[y - 1][x].visited) voisins.push_back(&grille[y - 1][x]); // Haut/ on vérifie qu'on n'est pas dans la première ligne->pas de cellule au desus
        if (y < rows - 1 && !grille[y + 1][x].visited) voisins.push_back(&grille[y + 1][x]); // Bas/ on verifie qu on nest pas dans la derniere ligne->pas de cellule en bas

        // Si des voisins non visités existent
        if (!voisins.empty()) {
            // Choisir un voisin aléatoirement
            Cell* next = voisins[rand() % voisins.size()];

        // Supprimer le mur entre les cellules pour avoir des chemins(lignes)
        
            if (next->x == x - 1) { // Gauche
                current->leftWall = false;
                next->rightWall = false;
            } else if (next->x == x + 1) { // Droite
                current->rightWall = false;
                next->leftWall = false;
            } else if (next->y == y - 1) { // Haut
                current->topWall = false;
                next->bottomWall = false;
            } else if (next->y == y + 1) { // Bas
                current->bottomWall = false;
                next->topWall = false;
            }
            // Marquer le voisin comme visité et l'ajouter à la pile
            next->visited = true;
            stack.push(next);
        } else {
            // Retour en arrière
            stack.pop();
        }
    }
}

// Fonction pour dessiner le labyrinthe
//affiche les murs du lab a l ecran en itilisant raylib
void DessinerLabyrinthe() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int x = j * cellSize;
            int y = i * cellSize;

            // Dessiner les murs
            if (grille[i][j].topWall) DrawLine(x, y, x + cellSize, y, WHITE);
            if (grille[i][j].rightWall) DrawLine(x + cellSize, y, x + cellSize, y + cellSize, WHITE);
            if (grille[i][j].bottomWall) DrawLine(x, y + cellSize, x + cellSize, y + cellSize, WHITE);
            if (grille[i][j].leftWall) DrawLine(x, y, x, y + cellSize, WHITE);
        }
    }
}
 // Vérifier si le labyrinthe est résolu (en fonction de la position du joueur)
 //on verifie si le joueur a atteint la sortie
    bool VerifierSolution(int playerX, int playerY) {
        // Par exemple, vérifier si le joueur est arrivé en bas à droite
        return (playerX == cols - 1 && playerY == rows - 1);
    }
};

class Joueur{
    public:
    int x,y;//position
    Color couleur;
    int taille;

    //constructeur qui initialise la position du joueur
     Joueur() : x(0), y(0), couleur(BLUE), taille(0) {}

     void Initialiser(int cellSize) {
        taille = cellSize / 2;
    }
    //methode pour afficher le joueur
    void  Afficher(){
        int pixelX = x* cellSize + cellSize /2 ;
        int pixelY = y * cellSize + cellSize /2 ;
        DrawCircle(pixelX, pixelY, taille, couleur);
    }
    // Méthode pour déplacer le joueur
    //verifions qu il n y a pas de mur avant de permettre le mvt
    void Deplacer(vector<vector<Labyrinthe::Cell>>& grille,int cols ,int rows) {
        if (IsKeyPressed(KEY_RIGHT) && x < cols - 1 && !grille[y][x].rightWall) {
            x++;
        }
        if (IsKeyPressed(KEY_LEFT) && x > 0 && !grille[y][x].leftWall) {
            x--;
        }
        if (IsKeyPressed(KEY_UP) && y > 0 && !grille[y][x].topWall) {
            y--;
        }
        if (IsKeyPressed(KEY_DOWN) && y < rows - 1 && !grille[y][x].bottomWall) {
            y++;
        }
    }

};


class Niveau {
public:
    int rows, cols;

    // Méthode pour choisir le niveau
    void ChoisirNiveau(int niveau) {
        switch (niveau) {
            case 1:  // Facile
                rows = 10;
                cols = 10;
                break;
            case 2:  // Moyen
                rows = 15;
                cols = 15;
                break;
            case 3:  // Difficile
                rows = 20;
                cols = 20;
                break;
            default:  // Par défaut, niveau moyen
                rows = 15;
                cols = 15;
                break;
        }
    }
};




class Jeu {
public:
    int startTime, elapsedTime;
    int chrono;
    enum EtatJeu { MENU, EN_COURS, TERMINE };
    EtatJeu etat;
    int tempsFinal;

    Jeu() : startTime(0), elapsedTime(0), tempsFinal(0), etat(MENU) {}

    void DemarrerChrono() {
        startTime = GetTime();
    }
    

    void AfficherMenu() {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Menu Principal", 300, 200, 30, WHITE);
        DrawText("Appuyez sur 1 pour Niveau Facile", 230, 250, 20, WHITE);
        DrawText("Appuyez sur 2 pour Niveau Moyen", 230, 280, 20, WHITE);
        DrawText("Appuyez sur 3 pour Niveau Difficile", 230, 310, 20, WHITE);
        DrawText("Appuyez sur ESC pour Quitter", 230, 340, 20, WHITE);
        EndDrawing();
    }

void AfficherChrono() {
   
    if (etat == EN_COURS) {
    elapsedTime = GetTime() - startTime;  // Continue à calculer le temps lorsque le jeu est en cours
} else if (etat == TERMINE) {
    elapsedTime = tempsFinal;  // Affiche le temps final lorsque le jeu est terminé
}
DrawText(TextFormat("Temps: %d secondes", elapsedTime), 10, 10, 20, WHITE);

}



void AfficherVictoire() {

        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("Bravo, vous avez gagné !", 250, 250, 30, WHITE);
        DrawText(TextFormat("Temps écoulé : %d secondes", tempsFinal), 250, 300, 20, WHITE);

        DrawText("Appuyez sur ESC pour quitter", 250, 350, 20, WHITE);
        EndDrawing();
    }



};




int main() {
    InitWindow(screenWidth, screenHeight, "Labyrinthe");
    SetTargetFPS(60);

    Color black = {0, 0, 0, 255};
    Jeu jeu;
    Niveau niveau;
    Joueur joueur;

    // Choisir le niveau via le menu
    while (!WindowShouldClose() && jeu.etat == Jeu::MENU) {
        jeu.AfficherMenu();
        if (IsKeyPressed(KEY_ONE)) niveau.ChoisirNiveau(1);
        if (IsKeyPressed(KEY_TWO)) niveau.ChoisirNiveau(2);
        if (IsKeyPressed(KEY_THREE)) niveau.ChoisirNiveau(3);
        if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_THREE)) {
            jeu.etat = Jeu::EN_COURS;
             jeu.DemarrerChrono();  // Démarrer le chronomètre lorsque le jeu commence

        }
        if (IsKeyPressed(KEY_ESCAPE)) {
            CloseWindow();
            return 0;
        }
    }

    // Initialiser le labyrinthe
    cellSize = screenWidth / niveau.cols;
    joueur.Initialiser(cellSize);


    Labyrinthe lab(niveau.rows, niveau.cols);
    lab.GenererLabyrinthe();

    // Boucle principale du jeu
     while (!WindowShouldClose()) {
        if (jeu.etat == Jeu::EN_COURS) {
            joueur.Deplacer(lab.grille, niveau.cols, niveau.rows);
    
        if (joueur.x == niveau.cols - 1 && joueur.y == niveau.rows - 1) {
            jeu.tempsFinal = GetTime() - jeu.startTime;  // Enregistrer le temps final

            jeu.etat = Jeu::TERMINE;
        }
        BeginDrawing();
        ClearBackground(black);

        // Dessiner le labyrinthe
        lab.DessinerLabyrinthe();

        // Afficher le joueur
        joueur.Afficher();
        // Dessiner l'entrée et la sortie
        DrawRectangle(0, 0, cellSize, cellSize, GREEN);  // Entrée
        DrawRectangle((niveau.cols - 1) * cellSize, (niveau.rows - 1) * cellSize, cellSize, cellSize, RED);  // Sortie

        // Afficher le chrono
        jeu.AfficherChrono();



        EndDrawing();


         } else if (jeu.etat == Jeu::TERMINE) {
            jeu.AfficherVictoire();
            if (IsKeyPressed(KEY_ESCAPE)) break;
        }
    }

    CloseWindow();
    return 0;
}