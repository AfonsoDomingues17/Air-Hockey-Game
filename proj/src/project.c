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
#include "controller/timer/timer.h"
#include "controller/keyboard/keyboard.h"
#include "controller/mouse/mouse.h"
#include "controller/graphics/graphics.h"
#include "controller/rtc/rtc.h"
#include "model/model.h"
#include "view/view.h"
#include "controller/serialport/serialport.h"

extern MainStateMachine mainState;



int main(int argc, char *argv[]) {
    lcf_set_language("EN-US");

    lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

    lcf_log_output("/home/lcom/labs/proj/src/output.txt");

    if (lcf_start(argc, argv)) return 1;
    
    lcf_cleanup();

    return 0;

}

int init() {
  /* Initializes the video module in graphics mode */ 
  if (vg_init(0x14C) == NULL) return 1;

  /* Subscribe interrupts */
  if (serialPort_init() != 0) return 1;
  if (timer_subscribe_interrupt()) return 1;
  if (keyboard_subscribe_int()) return 1;
  if (mouse_enable_reporting()) return 1;
  if (mouse_subscribe_int()) return 1;
  if (rtc_subscribe()) return 1;
  if (sp_subscribe_int()) return 1;

  /* Create Interactable Objects */
  loader_sprite();

  /* Draw background */
  draw_frame();

  return 0;
}

int finalize() {
  /* Exit graphic mode */
  if(vg_exit() != 0) return 1;

  /* Unsubscrive interrupts */
  if (timer_unsubscribe_interrupt()) return 1;
  if (keyboard_unsubscribe_int()) return 1;
  if (mouse_unsubscribe_int()) return 1;
  if (mouse_disable_reporting()) return 1;
  if (rtc_unsubscribe()) return 1;
  if (sp_unsubscribe_int()) return 1;
  if (serialPort_exit() != 0) return 1;

  /* Destroy Interactable Objects */
  unloader_sprite();

  return 0;
}

int (proj_main_loop)(int argc, char **argv) {
  /* Run setup */
  if (init()) return 1;

  /* Device loop */
  int ipc_status, r;
  message msg;

  while (mainState != STOP) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */                
          if (msg.m_notify.interrupts & TIMER_MASK) timer_int();
          if (msg.m_notify.interrupts & KEYBOARD_MASK) keyboard_int();
          if (msg.m_notify.interrupts & MOUSE_MASK) mouse_int();
          if (msg.m_notify.interrupts & RTC_MASK) rtc_int();
          if (msg.m_notify.interrupts & SPMASK) sp_int();
          break;
        default:
          break; /* no other notifications expected: do nothing */    
      }
    } else { /* received a standard message, not a notification */
      /* no standard messages expected: do nothing */
    }
  }

  /* Exit Safelly */
  if (finalize()) return 1;

  return 0;
}
