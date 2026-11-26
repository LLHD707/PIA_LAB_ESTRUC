#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <windows.h> // Para SetConsoleOutputCP (acentos en la consola de Windows)
#endif
#include "usuario.h"
#include "tareas.h"
#include "materias.h"
#include "horario.h"
#include "pomodoro.h"
#include "gastos.h"


int Iniciar_Sesion(char nombre[], char contra[]){//NO MOVER NADA 0-0
    char sesion;
    char confcontra[50];
    
    do { //Ciclo para que decida si iniar sesion o un registro
        printf("\033[1m¿Desea iniciar sesión? \033[0m\tIngresa \033[1mY\033[0m para iniciar, \033[1mN\033[0m para registrarse: \n");
        scanf(" %c", &sesion);
        //No c como ponerlo, pero en si es y=Y y n=N
        if (sesion == 'y') sesion = 'Y';
        if (sesion == 'n') sesion = 'N';
        if (sesion != 'Y' && sesion != 'N') {
            printf("Opción inválida. Inténtalo de nuevo.\n");
        }
    } while (sesion != 'Y' && sesion != 'N');

    if (sesion == 'Y') {//Inicio de sesion :v
        printf("\n\033[1mINICIO DE SESIÓN\033[0m\n");
        printf("Ingrese su nombre: \n");
        scanf(" %49s", nombre);
        printf("Ingrese la contraseña:\n");
        scanf(" %49s", contra);
        //Archivo de los usuarios 
        FILE *Usuarios = fopen("usuarios.txt", "r");
        if (Usuarios == NULL) {
            printf("\n\033[1;31mAún no hay usuarios registrados en el sistema. Regístrese primero.\033[0m\n");
            return 0; 
        }
        char archivo_nombre[50];
        char archivo_contra[50];
        int encontrado = 0;
        while(fscanf(Usuarios, "%49s %49s", archivo_nombre, archivo_contra) == 2) {//Checando si el usuario ya existe junto a su contraseña
            if(strcmp(archivo_nombre, nombre) == 0 && strcmp(archivo_contra, contra) == 0) {//el strcmp es pa checar si es que si existe coincidencias
                encontrado = 1;
                break;
            }
        }
        fclose(Usuarios);

        if (encontrado) {
            printf("\n\033[1;32m¡Inicio de sesión exitoso! Bienvenido de nuevo, %s.\033[0m\n", nombre);
            return 1; 
        } else {
            printf("\n\033[1;31mError: Usuario o contraseña incorrectos.\033[0m\n");
            return 0;
        }

    } else {//Registrando una nueva seccion pa el usuario
        printf("\n\033[1mREGISTRANDO NUEVA SESIÓN\033[0m\n");
        printf("Ingrese un nombre: \n");
        scanf(" %49s", nombre);
        if (!nombre_valido(nombre)) {//El nombre se usa en los archivos del usuario, por eso solo letras, numeros y _
            printf("\n\033[1;33mEl nombre solo puede tener letras, numeros y guion bajo (_).\033[0m\n");
            return 0;
        }
        char archivo_nombre[50];
        char archivo_contra[50];
        FILE *Usuarios = fopen("usuarios.txt", "r");
        int encontradoo = 0;
        if (Usuarios != NULL) { //Revisa si el archivo ya existe
            while(fscanf(Usuarios, "%49s %49s", archivo_nombre, archivo_contra) == 2) {
                if(strcmp(archivo_nombre, nombre) == 0) {
                    encontradoo = 1;
                    break;
                }
            }
            fclose(Usuarios);
        }
        if (encontradoo) {//Encontro el usuario ya existente 
            printf("\n\033[1;33m¡Error, Usuario: %s, ya establecido!\033[0m\n", nombre); 
            return 0; 
        }
        do {//Confirmando contraseña
            printf("Ingrese una contraseña:\n");
            scanf(" %49s", contra);
            printf("Confirme la contraseña:\n");
            scanf(" %49s", confcontra);
            if (strcmp(contra, confcontra) != 0) {
                printf("\nLas contraseñas no coinciden. Inténtalo de nuevo.\n");
            }
        } while (strcmp(contra, confcontra) != 0);
        //Agregando al usuario y su contraseña al archivo
        Usuarios = fopen("usuarios.txt", "a");
        if (Usuarios == NULL) {
            printf("Error con el archivo usuarios.txt al intentar guardarlo.\n");
            return 0;
        }
        fprintf(Usuarios, "%s %s\n", nombre, contra);
        fclose(Usuarios);
        printf("\n\033[1;32m¡Usuario registrado y guardado exitosamente!\033[0m\n");
        return 1; 
    }
}

void Cambiar_Nombre(char nombre_actual[]) {// Cambiando el nombre del usuario
    char nuevo_nombre[50];
    char archivo_nombre[50];
    char archivo_contra[50];
    int validando;
    char preguntando;
    do { //Ciclo para preguntar si es que esta segur@
        printf("¿Desa cambiar el nombre actual? Ingresa \033[1mY\033[0m o \033[1mN\033[0m\n");
        scanf(" %c", &preguntando);
        if (preguntando == 'y') preguntando = 'Y';
        if (preguntando == 'n') preguntando = 'N';
        if (preguntando != 'Y' && preguntando != 'N') {
            printf("Opción inválida. Inténtalo de nuevo.\n");
            }
    } while (preguntando != 'Y' && preguntando != 'N');
    if(preguntando== 'N'){
        return;
    }
    printf("Nombre actual: %s\n", nombre_actual);
    FILE *Usuarios = fopen("usuarios.txt", "r");
    if (Usuarios == NULL) {
        printf("\n\033[1;31mError al abrir usuarios.txt.\033[0m\n");
        return;
    }
    do{
        validando=1;
        printf("Ingrese el nuevo nombre de usuario: ");
        scanf(" %49s", nuevo_nombre);
        if (!nombre_valido(nuevo_nombre)) {//Solo letras, numeros y _ (se usa en los archivos del usuario)
            printf("\n\033[1;33mEl nombre solo puede tener letras, numeros y guion bajo (_).\033[0m\n");
            validando=0;
        } else if (strcmp(nombre_actual, nuevo_nombre) == 0) {// Checando si escribio el mismo nombre
            printf("\n\033[1;33mEl nuevo nombre es igual al actual. Ingrese otro nombre.\033[0m\n");
            validando=0;
        }else{
            rewind(Usuarios);
            while (fscanf(Usuarios, "%49s %49s", archivo_nombre, archivo_contra) == 2) {// Verificando que el nuevo nombre no exista ya
                if (strcmp(archivo_nombre, nuevo_nombre) == 0) {
                    printf("\n\033[1;33mError: El usuario '%s' ya está en uso.\033[0m\n", nuevo_nombre);
                    validando=0;
                    break;
                }
            }
        }
    } while (validando==0);
    rewind(Usuarios);//Mandamos a la linea 1
    FILE *Temp = fopen("temp.txt", "w");// Archivo temporal para reescribir los datos
    if (Temp == NULL) {
        printf("\n\033[1;31mError al crear archivo temporal.\033[0m\n");
        fclose(Usuarios);
        return;
    }
    while (fscanf(Usuarios, "%49s %49s", archivo_nombre, archivo_contra) == 2) {
        if (strcmp(archivo_nombre, nombre_actual) == 0) {
            fprintf(Temp, "%s %s\n", nuevo_nombre, archivo_contra);//Escribiendo el nuevo nombre 
        } else {
            fprintf(Temp, "%s %s\n", archivo_nombre, archivo_contra);//Copiando los demas usuarios
        }
    }
    fclose(Usuarios);
    fclose(Temp);
    //Remplazando el archivo original por el temporal
    remove("usuarios.txt");
    rename("temp.txt", "usuarios.txt");
    renombrar_archivos_usuario(nombre_actual, nuevo_nombre);//Cambiando los nombres de los archivos del usuario al nuevo nombre
    establecer_usuario(nuevo_nombre);//Desde ahora los modulos guardan con el nombre nuevo
    strcpy(nombre_actual, nuevo_nombre);//Actualizando el nombre
    printf("\n\033[1;32m¡Nombre de usuario actualizado exitosamente a '%s'!\033[0m\n", nombre_actual);
}

void Cambiar_Contra(char nombre_actual[], char contra_actual[]) {// Cambiando el nombre del usuario
    char nueva_contra[50];
    char archivo_nombre[50];
    char archivo_contra[50];
    int validando;
    char preguntando;
    do { //Ciclo para preguntar si es que esta segur@
        printf("¿Desea cambiar la contraseña actual? Ingresa \033[1mY\033[0m o \033[1mN\033[0m\n");
        scanf(" %c", &preguntando);
        if (preguntando == 'y') preguntando = 'Y';
        if (preguntando == 'n') preguntando = 'N';
        if (preguntando != 'Y' && preguntando != 'N') {
            printf("Opción inválida. Inténtalo de nuevo.\n");
        }
    } while (preguntando != 'Y' && preguntando != 'N');
    if(preguntando== 'N'){
        return;
    }
    printf("Contraseña actual: %s\n", contra_actual);
    FILE *Usuarios = fopen("usuarios.txt", "r");
    if (Usuarios == NULL) {
        printf("\n\033[1;31mError al abrir usuarios.txt.\033[0m\n");
        return;
    }
    do{
        validando=1;
        printf("Ingrese la nuevo contraseña del usuario: ");
        scanf(" %49s", nueva_contra);
        if (strcmp(contra_actual, nueva_contra) == 0) {// Checando si escribio la misma contraseña
            printf("\n\033[1;33mLa nueva contraseña es igual al actual. Ingrese otra contraseña.\033[0m\n");
            validando=0;
        }
    } while (validando==0);
    rewind(Usuarios);//Mandamos a la linea 1
    FILE *Temp = fopen("temp.txt", "w");// Archivo temporal para reescribir los datos
    if (Temp == NULL) {
        printf("\n\033[1;31mError al crear archivo temporal.\033[0m\n");
        fclose(Usuarios);
        return;
    }
    while (fscanf(Usuarios, "%49s %49s", archivo_nombre, archivo_contra) == 2) {
        if (strcmp(archivo_nombre, nombre_actual) == 0) {
            fprintf(Temp, "%s %s\n", archivo_nombre, nueva_contra); //Poniendo la contra nueva
        } else {
            fprintf(Temp, "%s %s\n", archivo_nombre, archivo_contra);//Agregando lo demas 
        }
    }
    fclose(Usuarios);
    fclose(Temp);
    //Remplazando el archivo original por el temporal
    remove("usuarios.txt");
    rename("temp.txt", "usuarios.txt");
    strcpy(contra_actual, nueva_contra);//Actualizando la contraseña
    printf("\n\033[1;32m¡Contraseña del usuario actualizado exitosamente a '%s'!\033[0m\n", contra_actual);
}

int Eliminar_Cuenta(char nombre_actual[]) {//Eliminando la cuenta
    char archivo_nombre[50];
    char archivo_contra[50];
    char confirmar;
        do {//Preguntando si es que quiere eliminar su sesion
            printf("\033[1;36m¿Seguro que desea Eliminar la sesion\033[0m? (\033[1mY\033[0m/\033[1mN\033[0m): ");
            scanf(" %c", &confirmar);
            if (confirmar == 'y') confirmar = 'Y';
            if (confirmar == 'n') confirmar = 'N';
            if (confirmar != 'Y' && confirmar != 'N') {
                printf("Opción inválida. Inténtalo de nuevo.\n");
            }
        } while (confirmar != 'Y' && confirmar != 'N');
        
        if(confirmar=='Y'){
            char confirmacion_doble;
            do {//Preguntando de nuevo si es que esta seguro de eliminar todo
                printf("\033[1;35mEstas a punto de eliminar toda la informacion de tu cuenta ¿Seguro que desea Eliminar todo\033[0m? (\033[1mY\033[0m/\033[1mN\033[0m): ");
                scanf(" %c", &confirmacion_doble);
                if (confirmacion_doble == 'y') confirmacion_doble = 'Y';
                if (confirmacion_doble == 'n') confirmacion_doble = 'N';
                if (confirmacion_doble != 'Y' && confirmacion_doble != 'N') {
                    printf("Opción inválida. Inténtalo de nuevo.\n");
                }
            } while (confirmacion_doble != 'Y' && confirmacion_doble != 'N');
            if(confirmacion_doble=='Y'){// si puso Y en la primera condicion:
                FILE *Usuarios = fopen("usuarios.txt", "r");
                FILE *Temp = fopen("temp.txt", "w");
                if (Usuarios == NULL || Temp == NULL) {//Si puso Y es la confirmacion segunda
                    printf("\n\033[1;31mError al abrir los archivos.\033[0m\n");
                    if (Usuarios) fclose(Usuarios);
                    if (Temp) fclose(Temp);
                    return 0;
                }
                while (fscanf(Usuarios, "%49s %49s", archivo_nombre, archivo_contra) == 2) {//Copiendo a todos menos al usuario actual
                    if (strcmp(archivo_nombre, nombre_actual) != 0) {
                        fprintf(Temp, "%s %s\n", archivo_nombre, archivo_contra);
                    }
                }
                fclose(Usuarios);
                fclose(Temp);
                remove("usuarios.txt");
                rename("temp.txt", "usuarios.txt");
                eliminar_archivos_usuario(nombre_actual);// Borra las tareas, materias, horario, estudio y gastos del usuario
                printf("\n\033[1;32mLa cuenta '%s' ha sido eliminada del sistema.\033[0m\n", nombre_actual);
                return 1;
            }else{//Si puso N en la segunda condicion 
                return 0;
            }
        }else{
            return 0;//Si puso N en la primera condicon
        }
}

// Lee un entero del teclado. Si escriben otra cosa devuelve -1 (asi el menu no se cicla);
// si se cierra la entrada (EOF) devuelve valor_eof, que es la opcion de salir de ese menu
static int leer_entero(int valor_eof) {
    int v, c;
    int r = scanf("%d", &v);
    if (r == EOF) return valor_eof;
    while ((c = getchar()) != '\n' && c != EOF);//Descarta lo que sobre de la linea
    return (r == 1) ? v : -1;
}

// Carga los datos guardados del usuario que inicio sesion (cada modulo lee sus archivos)
static void cargar_datos_usuario(const char *nombre) {
    establecer_usuario(nombre);
    cargar_tareas();
    cargar_materias();
    cargar_horario();
    cargar_estudio();
    cargar_gastos();
}

// Libera la memoria de los modulos (los datos ya se guardaron al hacer cada cambio)
static void liberar_datos(void) {
    liberar_tareas();
    liberar_materias();
    liberar_estudio();
    liberar_gastos();
}

// Opcion 7: junta las estadisticas de todos los modulos
static void estadisticas(void) {
    printf("\n\033[1mESTADISTICAS\033[0m\n");
    printf("\n-- Tareas --\n");
    resumen_tareas();
    printf("\n-- Calificaciones --");
    mostrar_materias();
    float pg = promedio_general();
    if (pg >= 0) printf("\nPromedio general: %.1f\n", pg);
    printf("\n-- Estudio (Pomodoro) --\n");
    printf("Total estudiado: %.2f hrs\n", horas_estudiadas());
    printf("\n-- Gastos --");
    resumen_gastos();
}

// Opcion 8: resumen general del organizador
static void resumen(const char *nombre, const char *fecha_hora) {
    printf("\n\033[1mRESUMEN DE STUDYBOX\033[0m\n");
    printf("Usuario: %s\n", nombre);
    printf("Conectado desde: %s\n", fecha_hora);
    printf("Materias registradas: %d\n", total_materias());
    float pg = promedio_general();
    if (pg >= 0) printf("Promedio general: %.1f\n", pg);
    else printf("Promedio general: sin calificaciones\n");
    resumen_tareas();
    printf("Horas de estudio (Pomodoro): %.2f hrs\n", horas_estudiadas());
    resumen_gastos();
}

int main() {//AQUI ESTA EL MAIN-#################################################################################################################################################
    char nombre[50];
    char contrasena[50];
    char reintento;
    int intento=1 , EntrandoAlMenu=1;
    time_t inicio_sesion;
    char fecha_hora_inicio[30];

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // acentos y ñ al imprimir
    SetConsoleCP(CP_UTF8);       // y al leer lo que escribe el usuario
#endif

    printf("\033[1mBIENVENIDO A STUDYBOX, EL ORGANIZADOR DE TAREAS \"FCFM\".\033[0m\n");
    int acceso_permitido = Iniciar_Sesion(nombre, contrasena);
        if (acceso_permitido == 0) {//Preguntando si es que tiene acceso 
            EntrandoAlMenu = 0; 
            do{ //Salio un error y se le pregunta si es que quiere reintentarlo, en el iniciar sesion
                printf("\nNo se pudo acceder al sistema.\n");
                printf("¿Desea intentralo de nuevo?\n (\033[1mY\033[0m/\033[1mN\033[0m): ");
                scanf(" %c",&reintento);
                //No c como ponerlo, pero en si es y=Y y n=N, otra vez XD
                if (reintento == 'y') reintento = 'Y';
                if (reintento == 'n') reintento = 'N';
                if (reintento != 'Y' && reintento != 'N') {
                printf("Opción inválida. Inténtalo de nuevo.\n");
                }
                if(reintento=='Y'){//Pregunta si es que quiere reintentar 
                    printf("\n\033[1;31mADVERTENCIA. DESPUES DE 2 INTENTOS MÁS SE BLOQUEARA TODO\nInténtalo de nuevo.\033[0m\n");
                    printf("\n\033[1;31mADVERTENCIA. ES TU INTENTO: %d \033[0m\n",intento);
                    intento++;
                    acceso_permitido = Iniciar_Sesion(nombre, contrasena);
                    if (acceso_permitido == 1){
                        EntrandoAlMenu = 1; 
                        break; 
                    }
                }
                if(reintento=='N') {
                    intento+=100;
                    EntrandoAlMenu = 0;
                }
                    
            }while(intento<=3);

            if(EntrandoAlMenu == 0) {
                printf("\nSaliendo del Organizador de Tareas\n¡Hasta luego!\n");
            }
        }
    //AQUI ES DONDE ENTRA AL MENU AHORA SÍ-------------------------------------------------------------------------------------------------------------------------
    if(EntrandoAlMenu){
        int op;
        cargar_datos_usuario(nombre);//Carga los datos guardados de este usuario
        time(&inicio_sesion); //pide la hora actual
        struct tm *info_tiempo = localtime(&inicio_sesion);// acomodo la hora y la vuevle numeros separados
        strftime(fecha_hora_inicio, sizeof(fecha_hora_inicio), "%H:%M:%S %d/%m/%Y ", info_tiempo);
        do{
            printf("\n\033[1mMenu principal del Organizador de Tareas FCFM\033[0m\n");
            printf("Seleccione una opcion, escribiendo su número:\n");
            printf("\033[1m1\033[0m.-Materias.\n");
            printf("\033[1m2\033[0m.-Tareas.\n");
            printf("\033[1m3\033[0m.-Calificaciones.\n");
            printf("\033[1m4\033[0m.-Horario.\n");
            printf("\033[1m5\033[0m.-Gastos.\n");
            printf("\033[1m6\033[0m.-Pomodoro.\n");
            printf("\033[1m7\033[0m.-Estadisticas.\n");
            /* 
            Tareas completas, pendientes y vencidas
            Calificaiones de todas las materias 
            Horas de los dias
            Gastos hechos
            Tiempo del pomodoro al dia por dia
            */
            printf("\033[1m8\033[0m.-Resumen del Organizador.\n");
            /*
            Nombre del usuario
            Hora del usuario conectado
            Numero de materias:
            Tareas pendientes
            Tareas completadas
            calificaciones generales
            Numero de horas a la semana
            Gastos totales
            Horas hechas en el pomodoro
            */
            printf("\033[1m9\033[0m.-Consultar Sesión.\n");
            printf("\033[1m10\033[0m.-Salir.\n");
            op = leer_entero(10);
            switch (op) {
                case 1:
                    menu_materias();//Materias (el modulo guarda solo)
                    break;
                case 2:
                    menu_tareas();
                    break;
                case 3:
                    menu_materias();//Calificaciones: estan dentro del mismo modulo de materias
                    break;
                case 4:
                    menu_horario();
                    break;
                case 5:
                    menu_gastos();
                    break;
                case 6:
                    menu_pomodoro();
                    break;
                case 7:
                    estadisticas();
                    break;
                case 8:
                    resumen(nombre, fecha_hora_inicio);
                    break;
                case 9:{//  Consultar Sesion
                    int opsesion;
                    time_t ahora;
                    time(&ahora);
                    int segundos_totales = (int)difftime(ahora, inicio_sesion);// resta el tiempo pa calcular cuanto tiempo lleva activo
                    int minutos = segundos_totales / 60;
                    int segundos = segundos_totales % 60;
                    printf("Inicio de sesión: %s\n", fecha_hora_inicio);//Hora a la que inicio sesion
                    printf("Tiempo conectado: %d min %d seg\n", minutos, segundos);
                    printf("Nombre del usuario:\033[1m %s \033[0m\n", nombre);
                    do{//Submenu de la consulta de sesion
                        printf("\033[1m1\033[0m.-Ver contraseña actual.\n");
                        printf("\033[1m2\033[0m.-Cambiar nombre de usuario.\n");
                        printf("\033[1m3\033[0m.-Cambiar contraseña.\n");
                        printf("\033[1m4\033[0m.-Cambiar la Sesion.\n");
                        printf("\033[1m5\033[0m.-Eliminar  la sesion.\n");
                        printf("\033[1m6\033[0m.-Salir de la consulta de sesion.\n");
                        printf("Seleccione una opcion, escribiendo su número:\n");
                        opsesion = leer_entero(6);
                        switch (opsesion){
                        case 1:
                            printf("Contraseña actual:\033[1m %s \033[0m\n", contrasena);
                            break;
                        case 2:
                            Cambiar_Nombre(nombre);
                            break;
                        case 3:
                            Cambiar_Contra(nombre, contrasena);
                            break;
                        case 4: {
                            char confirmar;
                            do {
                                printf("¿Desea cerrar la sesión actual y cambiar de usuario? (\033[1mY\033[0m/\033[1mN\033[0m): ");
                                scanf(" %c", &confirmar);
                                if (confirmar == 'y') confirmar = 'Y';
                                if (confirmar == 'n') confirmar = 'N';
                                if (confirmar != 'Y' && confirmar != 'N') {
                                    printf("Opción inválida. Inténtalo de nuevo.\n");
                                }
                            } while (confirmar != 'Y' && confirmar != 'N');
                            if (confirmar == 'Y') {//Guardando los datos del usuario si es que falla al intentar iniciar otra sesion
                                char respaldo_nombre[50];
                                char respaldo_contra[50];
                                strcpy(respaldo_nombre, nombre);
                                strcpy(respaldo_contra, contrasena);
                                if (Iniciar_Sesion(nombre, contrasena) == 1) {//Reiniciando la hora de inicio para el nuevo usuario
                                    liberar_datos();//Se suelta lo del usuario anterior y se carga lo del nuevo
                                    cargar_datos_usuario(nombre);
                                    time(&inicio_sesion);
                                    struct tm *info_tiempo = localtime(&inicio_sesion);
                                    strftime(fecha_hora_inicio, sizeof(fecha_hora_inicio), "%H:%M:%S %d/%m/%Y ", info_tiempo);
                                    opsesion = 6;//Saliendo del submenu
                                } else {//Si fallo restauramos la sesión que tenía abierta                                   
                                    strcpy(nombre, respaldo_nombre);
                                    strcpy(contrasena, respaldo_contra);
                                    printf("\n\033[1;33mNo se pudo cambiar la sesión. Sesion Actual: %s.\033[0m\n", nombre);
                                }
                            }
                            break;
                        }
                        case 5:
                            if (Eliminar_Cuenta(nombre) == 1) {//No le pregunte que pusiera su contraseña porque en las opciones anteriores puede ver su contraseña y cambiarla 
                                opsesion = 6; // Sale del submenú de Consultar Sesión
                                op = 10;      // Sale del Menú Principal y cierra el programa
    }
                            break;
                        case 6:
                            printf("Saliendo del submenu consultar sesion.\n");
                            break;
                        default:
                            printf("\033[1;33mERROR.\033[0m \033[1mIngrese una de las opciones validas.\033[0m\n");
                            break;
                        }
                    } while (opsesion != 6);
                    break;
                }
                case 10:
                    printf("\nSaliendo...\n");
                    break;
                default:
                    printf("\033[1;33mERROR.\033[0m \033[1mIngrese una de las opciones validas.\033[0m\n");
                    break;
            }

        } while (op != 10);
        liberar_datos();
        printf("Regrese pronto.\n");
        
    
    }//FIN DEL MENU--------------------------------------------------------------------------------------------------------------------------------------------------
    
    return 0;
}


/*NO LE MUEVAN A ESTE CODIGO >:c*/
