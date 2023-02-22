// Adjy Sedar DESIR
// Bataille Navale
// ULCO 2022
#include <iostream>
#include <ctime>
using namespace std;


const int NBR_NAV = 6; // Nombre de navires
const int TAB_TAILLE_NAV[NBR_NAV] = {5,4,4,3,3,2}; // Tableau contenant la taille de mes 6 navires

struct Case{
	int lig, col;
};

struct navire{
	int taille = 0;
	Case *empl;
	bool *touche;
	int nbTouche = 0;
	bool couler = false;
};

struct flotte{
	navire ListeNavire[NBR_NAV];
	int nbreNavReel =NBR_NAV ;

};
struct Pile{
	Case Tab[100];
	int sommet = 0;
};

struct Liste{
Case TabL[100];
int taille = 0;
};

int alea(int a, int b); // Valeur aleatoire
int menuSens(); // menu choix de sens
bool **initGrille(int taille);//Initialise la grille
void Afficher(int taille, bool **Grille);
navire saisirNavire( flotte Fl, int taille, int indice);
flotte saisirFlotte( int taille);
navire saisirNavireORDI( flotte Fl, int taille, int indice);
flotte saisirFlotteOrdi( int taille);
void reponseOrdinateur(flotte *FlotteOrdi, Case PointsHumain, string *reponseOrdi,int taille, bool **GrilleOrdi);
void reponseHumain(flotte *FlotteHumain,Case PointOrdi, string *reponseHumain, int taille, bool **GrilleHumain);	
bool coupOrdinateur(Pile *p,flotte *FlotteHumain, int taille, bool **GrilleHumain);
bool coupHumain(  int taille, bool **GrilleOrdi,flotte *FlotteOrdi);
void miseAjour(Pile *CaseOrd,flotte *FlotteHumain,int taille, bool **GrilleHumain,bool **GrilleOrdi,flotte *FlotteOrdi,bool* hum );
bool choixJoueur();


int main(){
	srand(time(NULL));

	int taille = 10; // Taille de la grille
	int jr;
	
	bool joueur;
	bool **GrilleOrdi;
	bool **GrilleHumain;
	bool partie = true;
	
	flotte FlotteOrdi;
	flotte FlotteHumain;
	Pile CaseOrd;


	while(partie){
	
		joueur = choixJoueur(); // choix d'un joueur aleatoire
		
		
		GrilleOrdi = initGrille( taille); // initialise la grille de l'ordi
	
		GrilleHumain = initGrille( taille); // initialise la grille de l'humain
		
	 
	
		cout<<"\t\t\t\t\t Bataille Navale\t\t"<<endl;
		cout<<"Vous devrez jouer la colonne et la ligne Ex A1\n"<<endl;
		
		
		Afficher( taille, GrilleHumain);
	
		cout<<endl;
		cout<<"\t\t\tSaisir les navires! "<<endl;
		FlotteHumain = saisirFlotte(taille); // mise en place des navires dans la grille par l'humain
		
		
		FlotteOrdi = saisirFlotteOrdi(taille); // mise en place des navires dans la grille par l'ordi
	
		while(FlotteOrdi.nbreNavReel > 0 && FlotteHumain.nbreNavReel > 0){ // tant qu'il n'y a pas de gagnant la partie joue
			miseAjour(&CaseOrd,&FlotteHumain,taille, GrilleHumain,GrilleOrdi,&FlotteOrdi,&joueur);
		}
		if(FlotteOrdi.nbreNavReel == 0){
			cout<< "Le vainceur c'est l'Humain"<<endl;
		}
		else if(FlotteHumain.nbreNavReel == 0){
			cout<<"Le vainceur c'est l'ordi"<<endl;
		}
		

		cout<<"Voulez-vous rejouer? "<<endl;
		cout<<"1. Oui"<<endl;
		cout<<"2. Non"<<endl;
		cin>>jr;
		
		while( (jr!=1) && (jr!=2)){
			cout<<"Voulez-vous rejouer? "<<endl;
			cin>>jr;
		}
	
		if(jr==1){
			partie = true;		
		}
		else{
			partie = false;
		}
		
	}
}


/*#######################################################*/
bool choixJoueur(){ // Fonction qui choisi aleatoirement un joueur au debut
	int choix = alea(0,1);
	if(choix == 0){
		// Donc l'humain commence
		return true;
	}
		return false;
}

int menuSens(){ // Fonction qui renvoie le sens choisi 
	int sens;
	cout<<"Entrer le sens: "<<endl;
	cout<<"\t 1. Vers le haut"<<endl;
	cout<<"\t 2. Vers le bas"<<endl;
	cout<<"\t 3. Vers la gauche"<<endl;
	cout<<"\t 4. Vers la droite"<<endl;
	cin>>sens;

	if (sens<0||sens>4){
		cout<<"Erreur! "<<endl;
		return menuSens();
	}
	
	return sens;
}

bool **initGrille(int taille){ // Fonction qui initialise une grille a faux
  bool **Grille;
  Grille = new bool *[taille];
  for(int i=0; i<taille; i++){
    Grille[i]=new bool [taille];
    for (int j=0; j<taille; j++){
      Grille[i][j] = false;
    }
  }
  return Grille;
}


void Afficher(int taille, bool **Grille){ // Fonction qui affiche les valeurs de la grille
	cout<<"  ";
	for(int i=0;i<taille;i++){
		cout<<char( (int ('A') ) +i)<<" ";
		}
	cout<<endl;
	for(int i = 0; i<taille; i++){
		cout<<i<<" ";
		for(int j = 0; j<taille; j++){
			if((Grille[i][j]) == false){
				cout<<" ";
			}
			else if((Grille[i][j]) == true){
				cout<<"X";
			
			}
			cout<<" ";		
		}
		cout<<endl;
	}
}


navire saisirNavire( flotte Fl, int taille, int indice){ // Fonction qui permet de saisir les informations pour un navire
	int sens;
	navire Nav;
	Liste l;
	Case pos;
	char col;

	
	Nav.taille = TAB_TAILLE_NAV[indice]; // Recupere la taille du navire avec son indice dans le tableau navire
	Nav.empl = new Case[Nav.taille];
	Nav.touche = new bool[Nav.taille];
	cout<<"Navire "<<indice+1<<" Taille: "<<Nav.taille<<endl;
			
	// cout<<"Entrer x? ";
	// cin>>pos.lig;
	// cout<<"Entrer y? ";
	// cin>>col;
	cout<<"Entrer les coordonnees: ";
	cin>>col>>pos.lig;

	int temp = int(col);
	int a = int('A');
	if((temp<a) || (temp>a+taille)){
		cout<<"Erreur! ";
 		return saisirNavire(  Fl,  taille,  indice);
		 
	}

	for(int i=0; i<taille; i++){
		if(int('A')+i == temp){
			pos.col = i;
		}
	}

	if( (pos.lig<0) || (pos.col<0) || (pos.lig>=taille) || (pos.col>=taille) ){
		cout<<"Erreur! "<<endl;
		return  saisirNavire(  Fl,  taille,  indice);
	}

	
	l.TabL[l.taille] = pos; //Permet de sauvegarder les coordonnees du navire
	l.taille++;
		
	
	sens =  menuSens();  // recupere le sens

	// Verifie si mon navire va pas quitter la grille du jeu
	
	if( ((sens == 2) && (pos.lig + Nav.taille - 1 ) >= taille ) || ((sens == 4) &&(pos.col + Nav.taille - 1 ) >= taille ) || ((sens == 1) && (pos.lig - Nav.taille + 1 ) <0 ) || ( (sens == 3) && (pos.col - Nav.taille + 1 ) <0 ) ){
		
		cout<<"Erreur! "<<endl;
		return  saisirNavire(  Fl,  taille,  indice);
	}

		
	for(int i=1; i<Nav.taille;i++){
		if( sens == 1){
			pos.lig = pos.lig-1;
		}
		else if( sens == 2){
			pos.lig = pos.lig+1;
			}
		else if( sens == 3 ){
			pos.col = pos.col-1;
			}
		else if( (sens == 4) && (pos.col+1<taille) ){
			pos.col = pos.col+1;
		}
		
		l.TabL[l.taille] = pos; //Permet de sauvegarder les coordonnees du navire
		l.taille++;
			
	}
	for(int i=0; i<NBR_NAV; i++){ // Verifie si les coordonnes de mon navire (celle de la liste ) n'ont pas deja ete prises par un autre navire
		for(int j=0; j<Fl.ListeNavire[i].taille;j++){
			for(int k=0; k<l.taille; k++){
				if( (Fl.ListeNavire[i].empl[j].lig == l.TabL[k].lig) && (Fl.ListeNavire[i].empl[j].col == l.TabL[k].col) ){
					cout<<"Ereur! "<<endl;
					return saisirNavire( Fl, taille, indice); // recomence si c vrai
				}
			}
		}
	}
	
			
	for(int k=0; k<l.taille; k++){ // Sauvegarder les coordonnes de mon navire dans le tableau empl
		Nav.empl[k] = l.TabL[k];
		Nav.touche[k] = false;	
		
	}	
	return Nav;
}

flotte saisirFlotte( int taille){// Fonction qui permet de saisir l'ensemble des navire
	flotte Flotte;
	for(int i=0; i<NBR_NAV; i++){
		Flotte.ListeNavire[i] = saisirNavire( Flotte,  taille, i);
	}
	return Flotte;
}


navire saisirNavireORDI( flotte Fl, int taille, int indice){	
	navire Nav;
	Liste l;	
	Case pos;

	Nav.taille = TAB_TAILLE_NAV[indice];  // Recupere la taille du navire avec son indice dans le tableau navire
	Nav.empl = new Case[Nav.taille];
	Nav.touche = new bool[Nav.taille];
	
	//Place la 1ere coordonnee de son navire
	pos.lig = alea(0, taille-1); 
	pos.col = alea(0, taille-1);

	l.TabL[l.taille] = pos; // Savegarde les coordonnees du navire
	l.taille++;
	
	int sens =  alea(1, 4); // Choisi un sens aleatoirement

	if( ((sens == 2) && (pos.lig + Nav.taille - 1 ) >= taille ) || ((sens == 4) &&(pos.col + Nav.taille - 1 ) >= taille ) || ((sens == 1) && (pos.lig - Nav.taille + 1 ) <0 ) || ( (sens == 3) && (pos.col - Nav.taille + 1 ) <0 ) ){ // Verifie si c possible de placer ces coordonnes
		
		return  saisirNavireORDI(  Fl,  taille,  indice);
	}

		
	for(int i=1; i<Nav.taille;i++){ // Place les prochaines coordonnes du navire
		if( sens == 1){
			pos.lig = pos.lig-1;
		}
		else if( sens == 2){
			pos.lig = pos.lig+1;
		}
		else if( sens == 3 ){
			pos.col = pos.col-1;
		}
		else if( (sens == 4) && (pos.col+1<taille) ){
			pos.col = pos.col+1;
		}
		
		l.TabL[l.taille] = pos; // Savegarde les coordonnees du navire
		l.taille++;
			
	}
	for(int i=0; i<NBR_NAV; i++){
		for(int j=0; j<Fl.ListeNavire[i].taille;j++){
			for(int k=0; k<l.taille; k++){
				if( (Fl.ListeNavire[i].empl[j].lig == l.TabL[k].lig) && (Fl.ListeNavire[i].empl[j].col == l.TabL[k].col) ){// Verifie si les coordonnes de mon navire (celle de la liste ) n'ont pas deja ete prises par un autre navire
					
					return  saisirNavireORDI(  Fl,  taille,  indice);
					
				}
			}
		}
	}
			
	for(int k=0; k<l.taille; k++){// Sauvegarder les coordonnes de mon navire dans le tableau empl
		Nav.empl[k] = l.TabL[k];
		Nav.touche[k] = false;	//Mets les cases touche a false
		
	}	
	return Nav;
}

flotte saisirFlotteOrdi( int taille){
	flotte Flotte;
	for(int i=0; i<NBR_NAV; i++){
		Flotte.ListeNavire[i] = saisirNavireORDI( Flotte,  taille, i);
	}
	return Flotte;
}

void reponseOrdinateur(flotte *FlotteOrdi, Case PointsHumain, string *reponseOrdi,int taille, bool **GrilleOrdi){
	
	for(int i=0;i<NBR_NAV;i++){
		
		for(int j=0;j< FlotteOrdi->ListeNavire[i].taille;j++){
			
			if( FlotteOrdi->ListeNavire[i].touche[j]==false){
				
				if( (PointsHumain.lig == FlotteOrdi->ListeNavire[i].empl[j].lig) && (PointsHumain.col == FlotteOrdi->ListeNavire[i].empl[j].col) ){
					
					FlotteOrdi->ListeNavire[i].touche[j]=true;
					FlotteOrdi->ListeNavire[i].nbTouche++;
					*reponseOrdi = "toucher";
					
					if(FlotteOrdi->ListeNavire[i].nbTouche == FlotteOrdi->ListeNavire[i].taille){
						FlotteOrdi->ListeNavire[i].couler = true;
						FlotteOrdi->nbreNavReel--;						
						*reponseOrdi = "couler";		
						
					}
					if(*reponseOrdi == "toucher" ){
						cout<<"Toucher! "<<endl;
					}
					else if(*reponseOrdi == "couler" ){
						cout<<"Couler! "<<endl;
					break;
				}						
		}
			
				
		}
		
	}
	
		
	
}
		if( ( *reponseOrdi != "couler" ) && (  *reponseOrdi != "toucher" )){
				*reponseOrdi = "eau";	
				cout<<"Dans l'eau! "<<endl;
				
			}
	GrilleOrdi[PointsHumain.lig][PointsHumain.col] = true;
	}


// void reponseHumain(flotte *FlotteHumain,Case PointOrdi, string *reponseHumain, int taille, bool **GrilleHumain){

// 	int rep=0;
// 	cout<<"\t1. Toucher\n";
// 	cout<<"\t2. Couler\n";
// 	cout<<"\t3. Dans l'eau\n";

// 	while(rep<=0 || rep>3){
// 		cout<<"Choisir le numero: ";
// 		cin>>rep;
// 		switch(rep){
			
// 			case 1:
// 				cout<<"Toucher!"<<endl;
// 				for(int i=0;i<NBR_NAV;i++){
// 					for(int j=0; j<FlotteHumain->ListeNavire[i].taille;j++){
// 						if(FlotteHumain->ListeNavire[i].touche[j]==false){
// 							if((FlotteHumain->ListeNavire[i].empl[j].lig == PointOrdi.lig) && (FlotteHumain->ListeNavire[i].empl[j].col == PointOrdi.col)){
// 								FlotteHumain->ListeNavire[i].touche[j]=true;
// 								FlotteHumain->ListeNavire[i].nbTouche++;
// 								*reponseHumain = "toucher";
								
// 								if(FlotteHumain->ListeNavire[i].nbTouche == FlotteHumain->ListeNavire[i].taille){
// 									FlotteHumain->ListeNavire[i].couler = true;
// 								}
								
// 							}
							
// 						}
						
// 					}
// 				}
				
// 			break;
// 			case 2:
// 				cout<<"Couler!"<<endl;
// 				FlotteHumain->nbreNavReel--;
// 				*reponseHumain = "couler";
// 			break;

// 			case 3:
// 				cout<<"Dans l'eau!"<<endl;
// 				*reponseHumain = "eau";
// 			break;

// 			default:
// 				cout<<"Erreur! "<<endl;
// 				rep = 0;
			
// 		}
// 	}
// 	GrilleHumain[ PointOrdi.lig][PointOrdi.col] = true;
// }

void reponseHumain(flotte *FlotteHumain, Case PointOrdi, string *reponseHumain,int taille, bool **GrilleHumain){


	
	
	for(int i=0;i<NBR_NAV;i++){
		
		for(int j=0;j< FlotteHumain->ListeNavire[i].taille;j++){
			
			if( FlotteHumain->ListeNavire[i].touche[j]==false){
				
				if( (PointOrdi.lig == FlotteHumain->ListeNavire[i].empl[j].lig) && (PointOrdi.col == FlotteHumain->ListeNavire[i].empl[j].col) ){
					
					FlotteHumain->ListeNavire[i].touche[j]=true;
					FlotteHumain->ListeNavire[i].nbTouche++;
					*reponseHumain = "toucher";
					
					if(FlotteHumain->ListeNavire[i].nbTouche == FlotteHumain->ListeNavire[i].taille){
						FlotteHumain->ListeNavire[i].couler = true;
						FlotteHumain->nbreNavReel--;						
						*reponseHumain = "couler";		
						
					}
					if(*reponseHumain == "toucher" ){
						cout<<"Toucher! "<<endl;
					}
					else if(*reponseHumain == "couler" ){
						cout<<"Couler! "<<endl;
					break;
				}						
		}
			
				
		}
		
	}
	
	
}
		if( ( *reponseHumain != "couler" ) && (  *reponseHumain != "toucher" )){
				*reponseHumain = "eau";	
				cout<<"Dans l'eau! "<<endl;
				
			}
	GrilleHumain[PointOrdi.lig][PointOrdi.col] = true;
	}

	
bool coupOrdinateur(Pile *CaseOrd,flotte *FlotteHumain, int taille, bool **GrilleHumain){
	int i,j;
	Case pos;
		string reponseH;
	if(CaseOrd->sommet==0){
		i = alea(0,taille-1);
		j = alea(0,taille-1);
		pos.lig = i;
		pos.col = j;
		
	}
		
	else{

		i = CaseOrd->Tab[CaseOrd->sommet-1].lig;
		j = CaseOrd->Tab[CaseOrd->sommet-1].col;
	}
	
	
	if( (GrilleHumain[i][j+1] ==false) && (j+1<taille) ){

		
		pos.lig = i;
		pos.col = j+1;
	}
		
	
	else if( (GrilleHumain[i][j-1] ==false) && (j-1 >= 0) ){

		
		pos.lig = i;
		pos.col = j-1;
	}
	
	
	else if( (GrilleHumain[i+1][j] ==false) && (i+1<taille) ){

		pos.lig = i+1;
		pos.col = j;
	}
		
	else if( (GrilleHumain[i-1][j] ==false) && (i-1>=0) ){

		
		pos.lig = i-1;
		pos.col = j;
	}
		
	else{

		if(CaseOrd->sommet != 0){
			CaseOrd->sommet--;		
		}
		i = alea(0,taille-1);
		j = alea(0,taille-1);
		pos.lig = i;
		pos.col = j;
		
		while( GrilleHumain[pos.lig][pos.col] == true){
			i = alea(0,taille-1);
			j = alea(0,taille-1);
			pos.lig = i;
			pos.col = j;
		}
	}

	cout<<"\t\t\t\t"<< char (int ('A')+pos.col)<<pos.lig<<endl;
	
	
 	reponseHumain( FlotteHumain, pos,  &reponseH,  taille,  GrilleHumain);
	if(reponseH=="eau"){
		return true;
	}
	if( (reponseH =="couler") || (reponseH =="toucher") ){
		CaseOrd->Tab[CaseOrd->sommet] = pos;
		CaseOrd->sommet++;
		return false;
	
	}
		
				
}


bool coupHumain(  int taille, bool **GrilleOrdi,flotte *FlotteOrdi){
		Case Humain;
	char colTmp;
	
string reponseOrdi;
		

	cout<<"Entrer les coordonnees: ";
	cin>>colTmp>>Humain.lig;
	

	if ((Humain.lig >=taille) || (Humain.lig<0) ){
		cout<<"Reesayer!"<<endl;
		return coupHumain(   taille,  GrilleOrdi, FlotteOrdi);
		
	}
		
	int l = int(colTmp);
	int A =int('A');
	
	if( l < A|| l>A+taille  ){
		cout<<"Reesayer!"<<endl;
		return coupHumain(   taille,  GrilleOrdi, FlotteOrdi);
		
	}

	for(int i=0; i<taille; i++){
		if(int('A')+i == l){
			Humain.col = i;
		}
	}

	// 	cout<<"Entrer la col: ";
	// cin>>Humain.col;

	// while( (Humain.col >=taille) || (Humain.col<0)){
	// 	cout<<"Reesayer!"<<endl;
	// 	cout<<"Entrer la col: ";
	// 	cin>>Humain.col;
	// }
	
		if(GrilleOrdi[Humain.lig][Humain.col]==true){
			cout<<"Erreur! "<<endl;
			// return  coupHumain( Humain,  taille,  GrilleOrdi);
			return coupHumain(  taille, GrilleOrdi,FlotteOrdi);		
			
		}
cout<<endl;
	cout<<"\t\t\t\t"<< colTmp<<Humain.lig<<endl;
	
 reponseOrdinateur(FlotteOrdi, Humain,  &reponseOrdi, taille,  GrilleOrdi);

	
	if(reponseOrdi == "eau"){
		// cout<<"Inste"<<reponseOrdi;
		return false;
	}
	if( (reponseOrdi == "toucher") || (reponseOrdi == "couler") ){
		return true;
	}
	else{
		return false;
	}
	
}

void miseAjour(Pile *CaseOrd,flotte *FlotteHumain,int taille, bool **GrilleHumain,bool **GrilleOrdi,flotte *FlotteOrdi,bool *hum){

		 if(*hum){
			 
	cout<<"\tAu tour de l'Humain! "<<endl;
			 
			 
  // Afficher( taille, GrilleOrdi);
 Afficher( taille, GrilleOrdi);
			 
			 
			 *hum = coupHumain(   taille, GrilleOrdi, FlotteOrdi);
  // Afficher( taille, GrilleOrdi);
 Afficher( taille, GrilleOrdi);
			 			 
		 }
			 cout<<"\n\n";
	
			if(!*hum){
				
	cout<<"\tAu tour de l'ordinateur! "<<endl;
			 
 Afficher( taille, GrilleHumain);
  			 
			 
			 *hum = coupOrdinateur( CaseOrd,FlotteHumain,  taille, GrilleHumain);	
  // Afficher( taille, GrilleHumain);
 Afficher( taille, GrilleHumain);
				
			 

			 
			 
		 }
	 

	 }
	

int alea(int a, int b){	
	return rand()% (b-a+1)+a ;
}
