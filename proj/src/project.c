/*! \mainpage Air Hockey 
* 
* Este projeto é um jogo entre duas pessoas que simula uma mesa de Air Hockey. 
*
* Consiste numa área retangular vertical, onde existem duas “balizas” no topo e no fundo desta.
* Existe também um pequeno disco que livremente desliza pela área de jogo. 
*
* Este disco poderá ser empurrado pelos jogadores apenas através de um manípulo redondo a que têm 
* acesso, os manípulos de cada jogador serão controlados com o rato segurando o botão esquerdo. 
* 
* O objetivo do jogo é empurrar o disco com o manípulo de forma que deslize para dentro da baliza do oponente.
*
* Este projeto foi feito por:
* @author Afonso Domingues, Afonso Machado, Filipa Geraldes, Luís Arruda
*
* @date 22/04/2024
*/

#include <lcom/lcf.h>
#include "devices/graphics/graphics.h"
#include "devices/timer/timer.h"

/*XPM*/
#include "assets/xpm_background.h"
#include "assets/bullet.h"

int main(int argc, char *argv[]) {
    lcf_set_language("EN-US");

    lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

    lcf_log_output("/home/lcom/labs/proj/src/output.txt");

    if (lcf_start(argc, argv)) return 1;
    
    lcf_cleanup();

    return 0;

}

int (draw)(xpm_map_t xpm) {
  xpm_image_t img;
  xpm_load(xpm, XPM_INDEXED, &img);
  if(mset_frame_buffer(0x105) != 0) return 1;
  if(set_graphic_mode(0x105) != 0) return 1;
  if(draw_xpm(xpm, 0, 0) != 0) return 1;
  if(kbd_test_scan() != 0 ) return 1;
  if(vg_exit() != 0) return 1;
  return 0;
}

int (proj_main_loop)(int argc, char **argv) {

    draw((xpm_map_t)bullet_xpm);

    return 0;
}





