
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

U8G2_SH1107_SEEED_128X128_1_HW_I2C u8g2(
    U8G2_R0, U8X8_PIN_NONE
);

const unsigned long DURACION_CARA = 3000;
const unsigned long INTERVALO_FRAME = 80;

unsigned long ultimoFrame = 0;

// ----------------------------------
// FUNCIONES AUXILIARES
// ----------------------------------

// Boca curva sin usar drawArc.
// profundidad positiva: sonrisa
// profundidad negativa: tristeza
void bocaCurva(int cx, int y, int ancho, int profundidad) {
    for (int x = -ancho; x < ancho; x += 2) {
        int x2 = x + 2;

        int y1 = y + profundidad *
                 (ancho * ancho - x * x) /
                 (ancho * ancho);

        int y2 = y + profundidad *
                 (ancho * ancho - x2 * x2) /
                 (ancho * ancho);

        u8g2.drawLine(cx + x, y1, cx + x2, y2);
    }
}

void estrella(int x, int y, int r) {
    u8g2.drawLine(x-r, y, x+r, y);
    u8g2.drawLine(x, y-r, x, y+r);
    u8g2.drawPixel(x-2, y-2);
    u8g2.drawPixel(x+2, y+2);
}

void corazon(int x, int y) {
    u8g2.drawDisc(x-4, y, 5);
    u8g2.drawDisc(x+4, y, 5);
    u8g2.drawTriangle(
        x-9, y+1,
        x+9, y+1,
        x, y+12
    );
}

// Ojos normales con brillo o cerrados
void ojos(int dx, bool cerrados) {
    if (cerrados) {
        u8g2.drawLine(43, 60, 55, 60);
        u8g2.drawLine(73, 60, 85, 60);
        return;
    }

    u8g2.drawDisc(49 + dx, 60, 7);
    u8g2.drawDisc(79 + dx, 60, 7);

    // Brillos blancos dentro de los ojos
    u8g2.setDrawColor(0);
    u8g2.drawDisc(47 + dx, 57, 2);
    u8g2.drawDisc(77 + dx, 57, 2);
    u8g2.setDrawColor(1);
}

void baseCara() {
    // Cabeza
    u8g2.drawCircle(64, 68, 39);

    // Detalles laterales
    u8g2.drawPixel(28, 68);
    u8g2.drawPixel(100, 68);
}

// ----------------------------------
// 1. FELIZ
// ----------------------------------

void caraFeliz(unsigned long t) {
    baseCara();

    // Parpadeo periodico
    bool parpadeo = (t % 1800) < 120;

    // Movimiento de pupilas
    int dx = (t / 450) % 3 - 1;

    ojos(dx, parpadeo);

    // Cejas
    u8g2.drawLine(41, 45, 54, 43);
    u8g2.drawLine(74, 43, 87, 45);

    // Sonrisa
    bocaCurva(64, 79, 18, 10);

    // Mejillas
    u8g2.drawCircle(35, 77, 4);
    u8g2.drawCircle(93, 77, 4);

    // Destellos animados
    estrella(14, 35 + (t/300)%5, 5);
    estrella(111, 98 - (t/300)%5, 4);
}

// ----------------------------------
// 2. EMOCIONADA
// ----------------------------------

void caraEmocionada(unsigned long t) {
    // Cabeza con ligero movimiento
    int salto = (t / 180) % 2;
    int cy = 68 - salto * 2;

    u8g2.drawCircle(64, cy, 39);

    // Ojos muy grandes
    u8g2.drawDisc(48, cy-8, 10);
    u8g2.drawDisc(80, cy-8, 10);

    u8g2.setDrawColor(0);
    u8g2.drawDisc(45, cy-12, 3);
    u8g2.drawDisc(77, cy-12, 3);
    u8g2.setDrawColor(1);

    // Boca abierta y alegre
    u8g2.drawDisc(64, cy+15, 12);

    // Dientes
    u8g2.setDrawColor(0);
    u8g2.drawBox(57, cy+5, 14, 4);
    u8g2.setDrawColor(1);

    // Corazones flotantes
    corazon(12, 25 - (t/150)%12);
    corazon(109, 32 - (t/200)%12);

    estrella(111, 92, 5);
}

// ----------------------------------
// 3. SORPRENDIDA
// ----------------------------------

void caraSorprendida(unsigned long t) {
    baseCara();

    // Cejas levantadas
    u8g2.drawLine(39, 39, 55, 37);
    u8g2.drawLine(73, 37, 89, 39);

    // Ojos abiertos
    u8g2.drawCircle(48, 58, 11);
    u8g2.drawCircle(80, 58, 11);

    u8g2.drawDisc(48, 58, 4);
    u8g2.drawDisc(80, 58, 4);

    // Boca que cambia de tamano
    int radio = 7 + (t/250)%5;

    u8g2.drawDisc(64, 85, radio);

    // Brillo en la boca
    u8g2.setDrawColor(0);
    u8g2.drawDisc(61, 82, 2);
    u8g2.setDrawColor(1);

    // Rayos de sorpresa
    u8g2.drawLine(14, 45, 24, 51);
    u8g2.drawLine(114, 45, 104, 51);

    u8g2.drawLine(64, 12, 64, 22);
    u8g2.drawLine(48, 15, 51, 23);
    u8g2.drawLine(80, 15, 77, 23);
}

// ----------------------------------
// 4. TRISTE
// ----------------------------------

void caraTriste(unsigned long t) {
    baseCara();

    // Cejas inclinadas
    u8g2.drawLine(40, 42, 54, 49);
    u8g2.drawLine(74, 49, 88, 42);

    // Ojos
    u8g2.drawDisc(48, 61, 6);
    u8g2.drawDisc(80, 61, 6);

    // Brillos
    u8g2.setDrawColor(0);
    u8g2.drawPixel(46, 59);
    u8g2.drawPixel(78, 59);
    u8g2.setDrawColor(1);

    // Boca triste
    bocaCurva(64, 94, 15, -8);

    // Lagrima descendente
    int gotaY = 70 + (t/70)%27;

    u8g2.drawDisc(85, gotaY, 3);
    u8g2.drawTriangle(
        82, gotaY,
        88, gotaY,
        85, gotaY-7
    );

    // Pequeña nube decorativa
    u8g2.drawCircle(16, 20, 7);
    u8g2.drawCircle(25, 18, 9);
    u8g2.drawLine(10, 27, 31, 27);
}

// ----------------------------------
// 5. DORMIDA
// ----------------------------------

void caraDormida(unsigned long t) {
    baseCara();

    // Parpados cerrados
    u8g2.drawLine(39, 60, 55, 62);
    u8g2.drawLine(73, 62, 89, 60);

    // Pestanas
    u8g2.drawLine(39, 60, 36, 56);
    u8g2.drawLine(89, 60, 92, 56);

    // Boca pequeña
    u8g2.drawCircle(64, 84, 5);

    // Respiracion: movimiento de mejillas
    int r = 3 + (t/450)%3;

    u8g2.drawCircle(37, 77, r);
    u8g2.drawCircle(91, 77, r);

    // Letras Z ascendiendo
    int subida = (t/100)%25;

    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.drawStr(102, 65-subida, "Z");

    if (subida > 7) {
        u8g2.drawStr(111, 47-subida/2, "z");
    }

    // Luna decorativa
    u8g2.drawDisc(18, 21, 9);
    u8g2.setDrawColor(0);
    u8g2.drawDisc(23, 17, 8);
    u8g2.setDrawColor(1);
}

// ----------------------------------
// INTERFAZ Y ANIMACIONES
// ----------------------------------

void dibujarEscena(int estado, unsigned long t) {
    const char* nombres[] = {
        "FELIZ",
        "EMOCIONADA",
        "SORPRESA",
        "TRISTE",
        "DORMIDA"
    };

    // Cabecera
    u8g2.setFont(u8g2_font_6x12_tf);
    u8g2.drawStr(4, 11, "MACETA V0");

    // Indicador de animacion
    u8g2.drawStr(86, 11, nombres[estado]);

    // Caras
    switch (estado) {
        case 0: caraFeliz(t); break;
        case 1: caraEmocionada(t); break;
        case 2: caraSorprendida(t); break;
        case 3: caraTriste(t); break;
        case 4: caraDormida(t); break;
    }

    // Separador inferior
    u8g2.drawHLine(4, 114, 120);

    // Indicador de progreso
    int progreso = (t * 118UL) / DURACION_CARA;

    u8g2.drawFrame(4, 119, 120, 6);
    u8g2.drawBox(5, 120, progreso, 4);
}

// ----------------------------------
// SETUP Y LOOP
// ----------------------------------

void setup() {
    u8g2.begin();
}

void loop() {
    unsigned long ahora = millis();

    if (ahora - ultimoFrame < INTERVALO_FRAME) {
        return;
    }

    ultimoFrame = ahora;

    unsigned long ciclo = ahora % (DURACION_CARA * 5UL);

    int estado = ciclo / DURACION_CARA;

    unsigned long tiempoEstado = ciclo % DURACION_CARA;

    // Dibujado por paginas (Arduino Uno)
    u8g2.firstPage();

    do {
        dibujarEscena(estado, tiempoEstado);
    } while (u8g2.nextPage());
}
