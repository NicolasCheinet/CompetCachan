#include "Config.c"
#include "Define.c"
#include <xc.h>
#include "LCD.c"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <float.h>
#include <math.h>
#include "Variables.c"
caca
//Gestion du temps
unsigned long Get_Millis (void); //incr�mente de 1 � chaque milliseconde �coul�e
void Set_Millis (unsigned long t); //donne une nouvelle valeur au compteur de millisecondes
//Gestion du BP et encodeur rotatif
unsigned short Enc_Get_Value (void); //renvoie la valeur du compteur associ� � l'encodeur rotatif
void Enc_Set_Value (unsigned short val); //donne une nouvelle valeur au codeur associ� � l'encodeur rotatif
unsigned char BP_Get_Status (void); //renvoie le status du bouton poussoir
unsigned char BP_Get_Rising_Edge  (void); //renvoie 1 si front descendant, sinon 0
unsigned char BP_Get_Falling_Edge (void); //renvoie 1 si front montant, sinon 0
//Gestion des moteurs
void Motors_Enable(unsigned char enable); //enable/disable les moteurs
void Motors_Speed(signed char vitg, signed char vitd); //assigner vitesse aux moteurs, positif en marche avant, n�gatif en marche arri�re
void Motors_Turn_Angle (signed short angle); // tourne sur place de la valeur de angle, bloquant dans le main pour la dur�e de la manoeuvre
//Odometrie
signed long Motors_Get_Enc_Left (void); //renvoie le compteur de coups d'encodeurs du moteur gauche, incr�mente en marche avant, d�cr�mente en marche arri�re, 180 coups/tour
signed long Motors_Get_Enc_Right (void); //renvoie le compteur de coups d'encodeurs du moteur droit, incr�mente en marche avant, d�cr�mente en marche arri�re, 180 coups/tour
void Motors_Set_Enc_Left (signed long cpt); //donne une nouvelle valeur au compteur de coups d'encodeurs du moteur gauche
void Motors_Set_Enc_Right (signed long cpt); //donne une nouvelle valeur au compteur de coups d'encodeurs du moteur droit
//Gestion de la boussole
unsigned short Compass_Get_Angle_Magn(void); //renvoie l'angle magn�tique donn� par la boussole
//Gestion du haut-parleur
void NOTE (unsigned char note, signed char alteration, unsigned char octave, unsigned short duree); //Jouer une note entre C2 et C6, dur�e en ms, bloquant dans le main pour la dur�e de la note. Exemple : NOTE('D',1,4,230); --> R�# octave 4 pendant 230ms
void NOTE_Start (unsigned char note, signed char alteration, unsigned char octave); //Jouer une note, l'arr�t se fait avec la fonction NOTE_Stop()
void NOTE_Stop (void); //Arr�te la note en cours
//Gestion des entr�es analogiques
unsigned char ADC_Get_CNYG (void); //renvoie la valeur du capteur optique de gauche
unsigned char ADC_Get_CNYD (void); //renvoie la valeur du capteur optique de droite
unsigned char ADC_Get_BATT (void); //renvoie la tension batterie, 255=5V
//Gestion de l'�cran LCD
extern void LCD_Goto(char ligne, char colonne); //pour placer le curseur
void LCD_Retro (unsigned char retro); //allumer ou �teindre le retro �clairage du LCD
extern void LCD_Clear(void); //efface l'�cran
extern void LCD_Print(char *t); //affiche une chaine de caract�res
void LCD_Write_u8bits(unsigned char var, unsigned char digit); //�crire un 8 bits non sign� avec combien de digits, curseur plac� sur le digit le plus � gauche
void LCD_Write_s8bits(signed char var, unsigned char digit); //�crire un 8 bits sign� avec combien de digits, curseur plac� sur le digit le plus � gauche
void LCD_Write_u16bits(unsigned short var, unsigned char digit); //�crire un 16 bits non sign� avec combien de digits, curseur plac� sur le digit le plus � gauche
void LCD_Write_s16bits(signed short var, unsigned char digit); //�crire un 16 bits sign� avec combien de digits, curseur plac� sur le digit le plus � gauche
void LCD_Write_u32bits(unsigned long var, unsigned char digit); //�crire un 32 bits non sign� avec combien de digits, curseur plac� sur le digit le plus � gauche
void LCD_Write_s32bits(signed long var, unsigned char digit); //�crire un 32 bits sign� avec combien de digits, curseur plac� sur le digit le plus � gauche
//Gestion du lidar
void LIADR_Set_Search_Angle (unsigned short angle); //donner une valeur � l'angle central de recherche Lidar
unsigned short LIDAR_Get_Angle_min (void); //renvoie l'angle, entre 315� et 45�, o� le lidar voit la distance la plus courte
unsigned short LIDAR_Get_Angle_max (void); //renvoie l'angle, entre Search_angle+45 et Search_angle-45, o� le lidar voit la distance la plus longue
unsigned short LIDAR_Get_Distance_min (void); //renvoie la distance correspondant � Angle_min, minimum 25mm     depuis le centre du lidar
unsigned short LIDAR_Get_Distance_max (void); //renvoie la distance correspondant � Angle_max, maximum 16000mm  depuis le centre du lidar
//Gestion de la lecture/�criture en EEPROM
void DATAEE_WriteByte(unsigned short bAdd, unsigned char bData); //�crit bData � bAdd
unsigned char DATAEE_ReadByte(unsigned short bAdd); //renvoie la valeur stock�e � bAdd
//Gestion du BlueTooth
void BT_Send_Char (unsigned char var); //envoie un char au module BT
void BT_Send_String (unsigned char *str); //envoie une chaine de caract�res au module BT
unsigned char BT_RC_Enabled (void); //savoir si la r�c�ption depuis la t�l�commande BT a �t� autoris�e. Si oui, les moteurs sont � l'arr�t tant que la t�l�commande n'est pas connect�e
unsigned char BT_RC_Connected (void);  //savoir si le BT est connect� � la telecommande BT
//Gestion du flag d'arriv�e
void FLAG_Set (void);
//Gestion du d�part
unsigned char Get_Start_Switch (void); //renvoie 0 si le connecteur pour le d�part est branch� (RB7-GND), sinon 1

/*******  Utilisation de la t�l�commande RC (optionnel)  ******
Appairer le module : BT401-Audio A7:F6:9C:ED:F9:F0
APK Android T�l�commande RC disponible dans le r�pertoire
Ajuster les param�tres suivants :
- Settings : Continuous stream
- Change ranges :   - Maximum forward speed      : 90
                    - Minimum forward speed      : 0
                    - Stop Value                 : 0
                    - Maximum back speed         : 90
                    - Minimum back speed         : 0
                    - Maximum left angle wheels  : 180
                    - Center angle wheels        : 90
                    - Maximum right angle wheels : 0
                    - Le reste n'a pas d'importance
                    - Save as Temporary (tout en bas)
Connect to car : BT401-Audio A7:F6:9C:ED:F9:F0 */

#include "Fonctions.c"
#include "Init.c"
#include "Isr.c"

#define PI 3.14159265
#define ENCODER_OFFSET 2147483648                                               // D�calage encodeur
#define DIAMETRE_ROUES_MM 32.5                                                  // Diam�tre roue mm
#define ENCODEUR_CPT_PAR_TOUR 178.0                                             // Ticks par tour
#define ENTRAXE_ROUES_MM 106                                                    // Distance entre roues mm
#define MAX_POINTS 10

#define SEUIL_CNY 200                                                           // Defini le seuil de detection de l'arrivee
#define SEUIL_ARRIVEE 7000                                                      // Defini une distance minimale a parcourir pour atteindre l'arrivee

// Stocke les donn�es d'odom�trie
typedef struct{
    float distance_gauche_mm;                                                   // Distance totale roue gauche
    float distance_droite_mm;                                                   // Distance totale roue droite
    float theta_rad;                                                            // Orientation radian
    float theta_deg;                                                            // Orientation degr�
    float theta_ref_deg;                                                        // Orientation de r�f�rence
    float theta_calcul_deg;                                                     // Angle calcul� vers la cible
    long last_enc_g;                                                            // Derni�re valeur encodeur gauche
    long last_enc_d;                                                            // Derni�re valeur encodeur droite
    short distance_ref_g;                                                       // Distance ref gauche
    short distance_ref_d;                                                       // Distance ref droite
} Odometrie;
Odometrie odo = {0}; 

// Stocke les mesures Lidar
typedef struct {
    unsigned short distance_min;                                                // Distance la plus proche
    unsigned short distance_max;                                                // Distance la plus �loign�e
    unsigned short angle_min;                                                   // Angle de l'obstacle le plus proche
    unsigned short angle_max;                                                   // Angle de l'obstacle le plus lointain
} Lidar;
Lidar lidar = {0};

// Logs de positions
typedef struct {
    float x[MAX_POINTS];                                                        // Historique X
    float y[MAX_POINTS];                                                        // Historique Y
    unsigned int index;                                                         // Position courante
} PositionLog;
PositionLog poslog = {0};

// Coordonn�es et direction cible
typedef struct {
    float dx;                                                                   // delta X cible - d�part
    float dy;                                                                   // delta Y cible - d�part
    long r_distance;                                                            // Distance droite entre points
    long x_start;                                                               // Point de d�part X
    long y_start;                                                               // Point de d�part Y
    short theta_heading;                                                        // Angle de rotation � effectuer
} Navigation;
Navigation nav = {0};

double angle_rad;
short delta_ref_d=0;

uint8_t etat = 0;

// Initialise les p�riph�riques et l'�tat
void init(void){
    Initialisation();
    while(PORTBbits.RB7 == 0) {NOP();}
    LCD_Clear();
    LCD_Retro(1);
    Motors_Enable(1);
    Motors_Set_Enc_Left(ENCODER_OFFSET);
    Motors_Set_Enc_Right(ENCODER_OFFSET);
    odo.last_enc_g = Motors_Get_Enc_Left() - ENCODER_OFFSET;
    odo.last_enc_d = Motors_Get_Enc_Right() - ENCODER_OFFSET;
}

// Met � jour l'odom�trie
void odo_uptade(void){
    signed long enc_g = (signed long)(Motors_Get_Enc_Left() - ENCODER_OFFSET);
    signed long enc_d = (signed long)(Motors_Get_Enc_Right() - ENCODER_OFFSET);

    long delta_enc_g = enc_g - odo.last_enc_g;
    long delta_enc_d = enc_d - odo.last_enc_d;

    float distance_par_tour = PI * DIAMETRE_ROUES_MM;
    
    float delta_dist_g = (delta_enc_g / ENCODEUR_CPT_PAR_TOUR) * distance_par_tour;
    float delta_dist_d = (delta_enc_d / ENCODEUR_CPT_PAR_TOUR) * distance_par_tour;

    odo.distance_gauche_mm += delta_dist_g;
    odo.distance_droite_mm += delta_dist_d;

    // Mise � jour de l'angle en radian
    odo.theta_rad += (delta_dist_d - delta_dist_g) / ENTRAXE_ROUES_MM;
    odo.theta_deg = (odo.theta_rad * 180.0 / PI);

    odo.last_enc_g = enc_g;
    odo.last_enc_d = enc_d;
}

// Met � jour tous les capteurs
void sensors_update(void){ 
    odo_uptade();
    lidar.distance_min = LIDAR_Get_Distance_min();
    lidar.angle_min = LIDAR_Get_Angle_min();
    lidar.distance_max = LIDAR_Get_Distance_max();
    lidar.angle_max= LIDAR_Get_Angle_max();
}

// Calcule l'angle et la distance vers le point cible
void where_to_go(long x1, long y1, long x2, long y2){
    nav.x_start = x1;
    nav.y_start = y1;
    nav.dx = x2 - x1;
    nav.dy = y2 - y1;
    odo.theta_calcul_deg = odo.theta_deg;

    // Calcul de l'angle vers la cible
    odo.theta_calcul_deg = atan2(nav.dy, nav.dx) * 180.0f / PI;  

    // Conversion angle entre 0 et 360
    if (odo.theta_calcul_deg > 180){
        nav.theta_heading = 360.0f - odo.theta_calcul_deg; 
    } else {
        nav.theta_heading = odo.theta_calcul_deg;
    }

    // Distance droite entre point de d�part et d'arriv�e
    nav.r_distance = (long)sqrt((float)(nav.dx * nav.dx + nav.dy * nav.dy));
}

// Sauvegarde la position de r�f�rence avant d�placement
void log_reference_position(void){
    sensors_update();
    odo.distance_ref_g = odo.distance_gauche_mm;
    odo.distance_ref_d = odo.distance_droite_mm;
    odo.theta_ref_deg = odo.theta_deg;
}

// Enregistre la position actuelle dans le log
void log_current_position(void){
    float delta_dist_g = odo.distance_gauche_mm - fabs(odo.distance_ref_g);
    float delta_dist_d = odo.distance_droite_mm - fabs(odo.distance_ref_d);
    float delta_d = (delta_dist_g + delta_dist_d) / 2.0;

    float x = nav.x_start + delta_d * cos(odo.theta_rad);
    float y = nav.y_start + delta_d * sin(odo.theta_rad);

    poslog.x[poslog.index] = x;
    poslog.y[poslog.index] = y;
    poslog.index++;
}

// Affiche les distances et l'angle sur l'�cran
void LCD_odo(void){
    LCD_Goto(0,0);
    LCD_Print("G:");
    LCD_Goto(0,2);
    LCD_Write_s16bits((signed short)odo.distance_gauche_mm, 5);

    LCD_Goto(0,8);
    LCD_Print("D:");
    LCD_Goto(0,10);
    LCD_Write_s16bits((signed short)odo.distance_droite_mm, 5);

    LCD_Goto(1,0);
    LCD_Print("Theta:");
    LCD_Goto(1,7);
    LCD_Write_s16bits((signed short)odo.theta_deg, 4);
    LCD_Print(" d");
}

// Affiche la derni�re position enregistr�e
void LCD_coord(void){
    LCD_Goto(0,0);
    LCD_Print("x:");
    LCD_Goto(0,2);
    LCD_Write_u16bits((unsigned short)poslog.x[0], 5);
    LCD_Goto(1,0);
    LCD_Print("y:");
    LCD_Goto(1,2);
    LCD_Write_u16bits((unsigned short)poslog.y[0], 5);
}

// Affiche les donn�es du Lidar
void LCD_lidar(void){
    LCD_Goto(0, 0);
    LCD_Print("D:");   
    LCD_Goto(0, 2);
    LCD_Write_u16bits(lidar.distance_min, 6);

    LCD_Goto(1, 0);
    LCD_Print("A:");   
    LCD_Goto(1, 2);
    LCD_Write_u16bits(lidar.angle_min, 6);

    LCD_Goto(0, 8);
    LCD_Print("D:");   
    LCD_Goto(0, 10);
    LCD_Write_u16bits(lidar.distance_max, 6);

    LCD_Goto(1, 8);
    LCD_Print("A:");   
    LCD_Goto(1, 10);
    LCD_Write_u16bits(lidar.angle_max, 6);
}

int main(void){
    init();
    where_to_go(0, 0, 300, 200);                                                // D�finit la cible
    Motors_Turn_Angle((short)nav.theta_heading);                                // Tourne vers la cible
    log_reference_position();
    Motors_Speed(15,15);                                                        

    while(1){
        sensors_update();
        LCD_Goto(0, 0);
        LCD_odo();  

        // Si distance atteinte ? arr�t et log
        if ((odo.distance_gauche_mm - odo.distance_ref_g) > nav.r_distance){
            Motors_Speed(0, 0);
            log_current_position();
            LCD_Clear();
            LCD_coord();
            while(1); 
        }
    }
    return 0;
}