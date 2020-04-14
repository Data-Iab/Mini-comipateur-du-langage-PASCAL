#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char Car_cour;
FILE *f;


int compteur = 0;
int id_longeur = 0;
int const_longeur = 0;
int Ligne_erreur =1;
int Analyse_lexical = 1;

char  mot_cle[][20]= {"program", "const", "var", "begin", "end", "if", "then", "while", "do",
                      "read", "write", "else","repeat","until","for","into","downto","case","of"};

char  symboles_speciaux[16]= {';', '.', '+', '-', '*', '/', ',', ':', '<', '>', '(', ')', ':', '{', '='};

char CODE_LEX_CHAR[][20] = {"PROGRAM_TOKEN", "CONST_TOKEN", "VAR_TOKEN", "BEGIN_TOKEN", "END_TOKEN", "IF_TOKEN",
                            "THEN_TOKEN", "WHILE_TOKEN", "DO_TOKEN", "READ_TOKEN", "WRITE_TOKEN","ELSE_TOKEN",
                            "REPEAT_TOKEN","UNTIL_TOKEN","FOR_TOKEN", "INTO_TOKREN","DOWNTO_TOKEN","CAS_TOKEN",
                            "OF_TOKEN","PV_TOKEN", "PT_TOKEN", "PLUS_TOKEN", "MOINS_TOKEN", "MULT_TOKEN", "DIV_TOKEN",
                            "VIR_TOKEN", "AFF_TOKEN", "INF_TOKEN", "INFEG_TOKEN", "SUP_TOKEN", "SUPEG_TOKEN", "DIFF_TOKEN",
                            "PO_TOKEN", "PF_TOKEN","PL_TOKEN", "FIN_TOKEN","EG_TOKEN","ERREUR_TOKEN", "ID_TOKEN", "NUM_TOKEN"};


typedef enum {PROGRAM_TOKEN,CONST_TOKEN,VAR_TOKEN,BEGIN_TOKEN,END_TOKEN,IF_TOKEN,THEN_TOKEN,WHILE_TOKEN,DO_TOKEN,READ_TOKEN,
WRITE_TOKEN,ELSE_TOKEN ,REPEAT_TOKEN,UNTIL_TOKEN,FOR_TOKEN,INTO_TOKEN,DOWNTO_TOKEN,CASE_TOKEN,OF_TOKEN,PV_TOKEN,PT_TOKEN,PLUS_TOKEN,MOINS_TOKEN,MULT_TOKEN,DIV_TOKEN,VIR_TOKEN,AFF_TOKEN,INF_TOKEN,INFEG_TOKEN,SUP_TOKEN,
SUPEG_TOKEN,DIFF_TOKEN,PO_TOKEN,PF_TOKEN,PL_TOKEN,FIN_TOKEN,EG_TOKEN,ERREUR_TOKEN,ID_TOKEN,NUM_TOKEN, COMMENT_TOKEN
}CODE_LEX;


typedef enum {ERR_CAR_INC, ERR_FICH_VID, ERR_ID_LONG, ERR_FICH_INTRO , ERR_CONST_LONG,PROGRAM_ERR, ID_ERR,PV_ERR,
                PT_ERR,EG_ERR,NUM_ERR,CONST_VAR_BEGIN_ERR,AFF_ERR,PF_ERR, BEGIN_ERR, END_ERR, COND_ERR, THEN_ERR,
                SYNT_ERR, DO_ERR, PO_ERR, UNTIL_ERR,INTO_DOWNTO_ERR,OF_ERR,PL_ERR}Erreurs;


typedef struct {Erreurs CODE_ERR;
                char mes[40];
                }Erreurs2;

Erreurs2 MES_ERR[] = {{ERR_CAR_INC,"caractere inconnu"},
                    {ERR_FICH_VID,"fichier vide"},
                    {ERR_ID_LONG,"IDF tres long"},
                    {ERR_FICH_INTRO,"fichier introuvable"},
                    {ERR_CONST_LONG, "CONSTF tres long"},
                    {PROGRAM_ERR,"Erreur de programme"},
                    {ID_ERR,"Erreur en ID"},
                    {PV_ERR,"Ajoutez ;"},
                    {PT_ERR,"Ajoutez point a la fin du programme"},
                    {EG_ERR,"Ajoutez une valeur aux constantes"},
                    {NUM_ERR,"Ajoutez un nombre"},
                    {CONST_VAR_BEGIN_ERR,"La syntaxe non valide"},
                    {AFF_ERR,"Ajoutez := dans l'affectation"},
                    {PF_ERR,"Une ')' est attendue"},
                    {BEGIN_ERR, "Pas de 'BEGIN'"},
                    {END_ERR,"Il manque 'end' a la fin du programme"},
                    {COND_ERR, "Condition incorrecte"},
                    {THEN_ERR,"'then' attendue apres if"},
                    {SYNT_ERR,"Erreur de la syntaxe"},
                    {DO_ERR, "'do' attendue apres la boucle"},
                    {PO_ERR,"Ajoutez les ()"},
                    {UNTIL_ERR,"'until' attendue apres 'repeat'"},
                    {INTO_DOWNTO_ERR,"'into' ou 'downto' manquante"},
                    {OF_ERR,"'of' attendue dans 'case'"},
                    {PL_ERR,"':' attendue dans 'case'"}
                    };


typedef struct {CODE_LEX CODE;
                char NOM[21];
                }TSym_Cour;


TSym_Cour SYM_COUR;

void Erreur(Erreurs ERR){
    int ind_err = ERR;
    printf("Dans la ligne %d ,erreur numero %d : %s \n",Ligne_erreur, ind_err, MES_ERR[ind_err].mes);
    getch();
    exit(1);
}



int est_separateur(){
    int resultat=0;
    if ((Car_cour == ' ') || (Car_cour == '\n') || (Car_cour == '\t'))
       resultat=1;
 return resultat;
}


int est_lettre(){
    int resultat = 0;
    if((Car_cour >= 'a') && (Car_cour <= 'z'))
        resultat = 1;
    return resultat;

}


void mot_cle_nom(){
    int i, id=38;
    for(i=0 ; i<20 ; i++){
        if(strcmp(SYM_COUR.NOM, mot_cle[i])==0)
            id = i;
    }
    SYM_COUR.CODE = (CODE_LEX)id;
}

int est_symbole_special(){
    int i,resultat = 0;
    if(memchr(symboles_speciaux, Car_cour, sizeof(symboles_speciaux)))
            resultat = 1;
    return resultat;

}

void Lire_Car(){
    Car_cour = tolower(fgetc(f));
}




void Lire_mot(){
    int i = 0;
    while((est_symbole_special() != 1) && (est_separateur() != 1) && (Car_cour != EOF)){
        SYM_COUR.NOM[i] = Car_cour;
        Lire_Car();
        i++;
        id_longeur++;
        if(id_longeur > 20 && Analyse_lexical == 1)
        {
            Erreur(ERR_ID_LONG);
        }
    }
    SYM_COUR.NOM[i] = '\0';
}


void Lire_nombre(){
    int i = 0;
    while((est_symbole_special() != 1) && (est_separateur() != 1) && (est_lettre() == 0) && (Car_cour != EOF)){
        SYM_COUR.NOM[i] = Car_cour;
        Lire_Car();
        i++;
        const_longeur++;
        if(const_longeur > 11 && Analyse_lexical == 1)
        {
            Erreur(ERR_CONST_LONG);
        }
    }
    SYM_COUR.NOM[i] = '\0';
    SYM_COUR.CODE = NUM_TOKEN;
}


void Lire_sym(){
    switch(Car_cour){
            case ';':
                SYM_COUR.CODE = PV_TOKEN;
                Lire_Car();
                break;
            case '.':
                SYM_COUR.CODE = PT_TOKEN;
                Lire_Car();
                break;
            case '+':
                SYM_COUR.CODE = PLUS_TOKEN;
                Lire_Car();
                break;
            case '-':
                SYM_COUR.CODE = MOINS_TOKEN;
                Lire_Car();
                break;
            case '*':
                SYM_COUR.CODE = MULT_TOKEN;
                Lire_Car();
                break;
            case '/':
                SYM_COUR.CODE = DIV_TOKEN;
                Lire_Car();
                break;
            case ',':
                SYM_COUR.CODE = VIR_TOKEN;
                Lire_Car();
                break;
            case '=':
                SYM_COUR.CODE = EG_TOKEN;
                Lire_Car();
                break;
            case '(':
                SYM_COUR.CODE = PO_TOKEN;
                Lire_Car();
                break;
            case ')':
                SYM_COUR.CODE = PF_TOKEN;
                Lire_Car();
                break;
            case EOF:
                SYM_COUR.CODE = FIN_TOKEN;
                Lire_Car();
                break;
            case '>':
                Lire_Car();
                if(Car_cour == '=')
                    SYM_COUR.CODE = SUPEG_TOKEN;
                else{SYM_COUR.CODE = SUP_TOKEN;}
                Lire_Car();
                break;
            case '<':
                Lire_Car();
                if(Car_cour == '='){
                    SYM_COUR.CODE = INFEG_TOKEN;
                    Lire_Car();}
                else if(Car_cour == '>'){
                    SYM_COUR.CODE = DIFF_TOKEN;
                    Lire_Car();}
                else{
                        SYM_COUR.CODE = INF_TOKEN;
                }
                break;
            case ':':
                SYM_COUR.CODE = PL_TOKEN;
                Lire_Car();
                if(Car_cour == '='){
                    SYM_COUR.CODE = AFF_TOKEN;
                    Lire_Car();}
                break;
            case '{':  //S1
                Lire_Car();
                if(Car_cour == '*'){
                    loop:  //S3
                        do{
                        Lire_Car();
                        }while(Car_cour != '*'); //S4
                        Lire_Car();
                        if(Car_cour == '}') //S5
                        {
                            Lire_Car();
                            SYM_COUR.CODE = COMMENT_TOKEN;
                        }
                        else{goto loop;}

                }
                else{
                    Erreur(ERR_CAR_INC); //S2

                    }

            break;
            }
}

void Sym_Suiv(){
        while(est_separateur() == 1){
            if(Car_cour == '\n'){
            Ligne_erreur++;}
            Lire_Car();
			}
        if(est_lettre() == 1){
            Lire_mot();
            mot_cle_nom();
            compteur++;
            }
        else if(isdigit(Car_cour)){
            Lire_nombre();
            compteur++;
            }
        else if((est_symbole_special() == 1) || Car_cour == EOF){
            Lire_sym();
            compteur++;
            }
        else{
            Erreur(ERR_CAR_INC);
        }
        if(SYM_COUR.CODE == COMMENT_TOKEN)
        {
            Sym_Suiv();
        }

}


void Test_Symbole (CODE_LEX lc, Erreurs COD_ERR){
    if(SYM_COUR.CODE == lc){
        Sym_Suiv();
    }
    else{
        Erreur(COD_ERR);
        }
}


void Affichier_TOKEN(){
    printf("%s\n",CODE_LEX_CHAR[SYM_COUR.CODE]);
}
//C:\Workplace\t.txt

void CONSTS(){
    switch (SYM_COUR.CODE)	{
    case CONST_TOKEN:
        Sym_Suiv();
        Test_Symbole(ID_TOKEN,ID_ERR);
        loop1:
            Test_Symbole(EG_TOKEN,EG_ERR);
            Test_Symbole(NUM_TOKEN,	NUM_ERR);
            Test_Symbole(PV_TOKEN,	PV_ERR);
            if(SYM_COUR.CODE == ID_TOKEN){
                Sym_Suiv();
                goto loop1;}

        break;
    case VAR_TOKEN:
        break;
    case BEGIN_TOKEN:
        break;
    default:

      Erreur(CONST_VAR_BEGIN_ERR) ;

      break;
}
}


void VARS()	{
    switch (SYM_COUR.CODE){
    case VAR_TOKEN:
        Sym_Suiv();
        Test_Symbole(ID_TOKEN,	ID_ERR);
        while (SYM_COUR.CODE==VIR_TOKEN){
            Sym_Suiv();
            Test_Symbole(ID_TOKEN,	ID_ERR);
            }
        Test_Symbole(PV_TOKEN,	PV_ERR);
		break;
    case BEGIN_TOKEN:
        break;
    default:
      Erreur(CONST_VAR_BEGIN_ERR) ;
      break;
    }
}

void INSTS(){
    Test_Symbole(BEGIN_TOKEN, BEGIN_ERR);
    INST();
    while(SYM_COUR.CODE != END_TOKEN && SYM_COUR.CODE != FIN_TOKEN)
    {
        Test_Symbole(PV_TOKEN,PV_ERR);
        INST();
    }
    Ligne_erreur++;
    Test_Symbole(END_TOKEN,END_ERR);
}

void INST(){
    switch(SYM_COUR.CODE){
        case BEGIN_TOKEN:
            INSTS();
            break;
        case ID_TOKEN:
            AFFEC();
            break;
        case IF_TOKEN:
            SI();
            break;
        case WHILE_TOKEN:
            TANTQUE();
            break;
        case WRITE_TOKEN:
            ECRIRE();
            break;
        case READ_TOKEN:
            LIRE();
            break;
        case REPEAT_TOKEN:
            REPETER();
            break;
        case FOR_TOKEN:
            POUR();
            break;
        case CASE_TOKEN:
            CAS();
            break;
        default:
            Erreur(SYNT_ERR);
    }

}


void CAS(){
    Sym_Suiv();
    Test_Symbole(ID_TOKEN,ID_ERR);
    Test_Symbole(OF_TOKEN,OF_ERR);
    Test_Symbole(NUM_TOKEN,NUM_ERR);
    Test_Symbole(PL_TOKEN,PL_ERR);
    INST();
    while(SYM_COUR.CODE == NUM_TOKEN)
    {
        Sym_Suiv();
        Test_Symbole(PL_TOKEN, PL_ERR);
        INST();
    }
    if(SYM_COUR.CODE == ELSE_TOKEN)
    {
        Sym_Suiv();
        INST();
    }
    Test_Symbole(END_TOKEN, END_ERR);

}


void POUR(){
    Sym_Suiv();
    Test_Symbole(ID_TOKEN,ID_ERR);
    Test_Symbole(AFF_TOKEN,AFF_ERR);
    Test_Symbole(NUM_TOKEN,NUM_ERR);
    if(SYM_COUR.CODE == INTO_TOKEN || SYM_COUR.CODE == DOWNTO_TOKEN)
    {
        Sym_Suiv();
    }
    else
    {
        Erreur(INTO_DOWNTO_ERR);
    }
    Test_Symbole(NUM_TOKEN,NUM_ERR);
    Test_Symbole(DO_TOKEN,DO_ERR);
    INST();

}

void REPETER(){
    Sym_Suiv();
    INST();
    Test_Symbole(UNTIL_TOKEN,UNTIL_ERR);
    COND();
}


void ECRIRE(){
    Sym_Suiv();
    Test_Symbole(PO_TOKEN,PO_ERR);
    EXPR();
    while(SYM_COUR.CODE == VIR_TOKEN)
    {
        Sym_Suiv();
        EXPR();
    }
    Test_Symbole(PF_TOKEN,PF_ERR);

}

void LIRE(){
    Sym_Suiv();
    Test_Symbole(PO_TOKEN,PO_ERR);
    Test_Symbole(ID_TOKEN,ID_ERR);
    while(SYM_COUR.CODE == VIR_TOKEN)
    {
        Sym_Suiv();
        Test_Symbole(ID_TOKEN,ID_ERR);
    }
    Test_Symbole(PF_TOKEN,PF_ERR);


}

void TANTQUE(){
    Sym_Suiv();
    COND();
    Test_Symbole(DO_TOKEN,DO_ERR);
    INST();



}




void SI(){
    Sym_Suiv();
	COND();
	Test_Symbole(THEN_TOKEN,THEN_ERR);
	INST();
	if(SYM_COUR.CODE == ELSE_TOKEN)
    {
        Sym_Suiv();
        INST();
    }


}

void COND(){
	EXPR();
	if((SYM_COUR.CODE==EG_TOKEN) || (SYM_COUR.CODE==DIFF_TOKEN) || (SYM_COUR.CODE==INF_TOKEN) || (SYM_COUR.CODE==SUP_TOKEN) || (SYM_COUR.CODE==INFEG_TOKEN) || (SYM_COUR.CODE==SUPEG_TOKEN)){
         Sym_Suiv();
         EXPR();
    }
    else{
        Erreur(COND_ERR);
    }
}


void AFFEC(){
	Test_Symbole(ID_TOKEN,ID_ERR);
	Test_Symbole(AFF_TOKEN,AFF_ERR);
	EXPR();
}

void EXPR(){
	TERM();
	while(SYM_COUR.CODE==MOINS_TOKEN || SYM_COUR.CODE==PLUS_TOKEN){
         Sym_Suiv();
         TERM();
	}
}



void TERM(){
		FACT();
	while(SYM_COUR.CODE==MULT_TOKEN||SYM_COUR.CODE==DIV_TOKEN){
         Sym_Suiv();
         FACT();
	}
}

void FACT(){
    switch (SYM_COUR.CODE){
    case ID_TOKEN:
        Test_Symbole(ID_TOKEN, ID_ERR);
        break;
    case NUM_TOKEN:
        Test_Symbole(NUM_TOKEN, NUM_ERR);
        break;
    case PO_TOKEN:
        Sym_Suiv();
        EXPR();
        Test_Symbole(PF_TOKEN, PF_ERR);
        break;
    default:
        Erreur(SYNT_ERR);

    }

}


void BLOCK(){
    CONSTS();
    VARS();
    INSTS();

}

void PROGRAM(){
    Test_Symbole(PROGRAM_TOKEN,	PROGRAM_ERR);
    Test_Symbole(ID_TOKEN, ID_ERR);
    Test_Symbole(PV_TOKEN, PV_ERR);
    BLOCK();
    Test_Symbole(PT_TOKEN, PT_ERR);
}



int main()
{
    char chemin [32];
    printf("\nENTRER LE CHEMIN DU FICHIER :");
    scanf("%s",chemin);
    f=fopen(chemin,"r");
    if (f==NULL){
        Erreur(ERR_FICH_INTRO);
        }

    else
    {
        Lire_Car();
        printf("\n************ANALYSEUR LEXICAL*************\n");
        while(Car_cour != EOF){
                Sym_Suiv();
                id_longeur = 0;
                const_longeur = 0;
                Affichier_TOKEN();
        }
        Sym_Suiv();
        if(compteur == 1)
            Erreur(ERR_FICH_VID);
        Affichier_TOKEN();
        printf("\nFin de l'analyse lexicale -- succes --\n");
        fclose(f);
        f=fopen(chemin,"r");
        printf("\n************ANALYSEUR SYNTAXIQUE*********\n");
        Ligne_erreur = 0;
        Analyse_lexical = 0;
        Lire_Car();
        Sym_Suiv();
        PROGRAM();
        if(SYM_COUR.CODE == FIN_TOKEN)
            printf("\nFin de l'analyse syntaxique -- succes --\n");
        else printf("\nProgramme erronée");
        }
}

