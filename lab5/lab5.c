// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Any header files included below this line should have been created by you

#include "i8042.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int (graphic_mode)(uint16_t mode) {
    // struct with a union of anonymous structs that allow access
    // the IA32 registers as 8-bit, 16-bit, 32-bit registers
    reg86_t r;
    // inicializar estrutura com valor 0 em tudo
    memset(&r, 0, sizeof(r));
    r.ah = 0x4F; // parte mais significativa de .ax
    r.al = 0x02; // parte menos significativa de .ax
    r.bx =  mode | BIT(14); // set bit 14: linear framebuffer
    r.intno = 0x10; // aceder à BIOS
    if (sys_int86(&r) != 0) {
        printf("set_vbe_mode: sys_int86() failed \n");
        return 1;
    }
    return 0;
}

// switch the video adapter to the graphics mode and then back again to the default text mode
int(video_test_init)(uint16_t mode, uint8_t delay) {
    // mudar para o graphic mode
    if (graphic_mode(mode) != 0) return 1;
    // esperar
    sleep(delay);
    // voltar ao modo texto
    if (vg_exit() != 0) return 1;
    return 0;
}

vbe_mode_info_t mode_info;
static void *frame_buffer; // address to which is mapped
static uint16_t hres;
static uint16_t vres;

int (set_frame_buffer)(uint16_t mode) {
    // informação sobre o mode
    memset(&mode_info, 0, sizeof(mode_info));
    if (vbe_get_mode_info(mode, &mode_info) != 0) return 1;

    hres = mode_info.XResolution;
    vres = mode_info.YResolution;

    // tamanho dos buffers
    // bits por pixel = 20 -> (20 + 7) / 8 = 27/8 = 3.375
    // 3 bytes é o valor mais próximo uma vez que 
    // 2 bytes = 16 bits < 20
    unsigned int bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
    unsigned int frameSize = mode_info.XResolution *
                             mode_info.YResolution *
                             bytesPerPixel;
    // mapear de physical para virtual address space
    int r;
    struct minix_mem_range mr;
    mr.mr_base = mode_info.PhysBasePtr; // physical base address pointer
    mr.mr_limit = mr.mr_base + frameSize; // limit
    if ((r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr) != 0)) {
        printf("sys_privctl (ADD_MEM) failed: %d\n");
        return 1;
    }
    // virtual allocation 
    frame_buffer = vm_map_phys(SELF, (void *)mr.mr_base, frameSize);
    if (frame_buffer == NULL) {
        printf("couldn’t map video memory");
        return 1;
    }
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    unsigned int bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
    uint8_t *fb = frame_buffer;
    // posição inicial no frame buffer na horizontal
    fb += (hres * y + x) * bytesPerPixel;
    // loop na horizontal
    for (uint16_t i = 0; i < len; i++) {
        // se posição está entre os limites
        if (x + i > mode_info.XResolution || y > mode_info.YResolution) return 1;
        // copiar cor para o frame buffer na posição atual
        if (memcpy(fb, &color, bytesPerPixel) == NULL) return 1;
        // mover frame buffer para o próximo pixel
        fb += bytesPerPixel;
    }
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    // por cada linha
    for (uint16_t i = 0; i < height; i++) {
        // desenhar linha horizontal na posição atual da linha
        if (vg_draw_hline(x, y + i, width, color) != 0) return 1;
    }
    return 0;
}


int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
    // 1. map the video memory to the process' address space
    if (set_frame_buffer(mode) != 0) return 1;
    // 2. change the video mode to that in its argument
    if (graphic_mode(mode) != 0) return 1;
    // 3. draw a rectangle
    if (vg_draw_rectangle(x, y, width, height, color) != 0) return 1;
    // 4. reset the video mode to Minix's default text mode 
    // and return, upon receiving the break code of the ESC 
    // key (0x81)
    if (kbd_test_scan() != 0) return 1;
    if (vg_exit() != 0) return 1;
    return 0;
}

/*
    EXPLICAÇÃO:

    X - cor específica 
    . - cor de fundo
    coordenadas (2,2)
    width = 5
    height = 3
    1 - vg_draw_hline(2, 2+0, 5, 'X')
    2 - vg_draw_hline(2, 2+1, 5, 'X')
    3 - vg_draw_hline(2, 2+2, 5, 'X')

    inicio:    1:         2:         3:
    .......    .......    .......    .......   
    .......    ..XXXXX    ..XXXXX    ..XXXXX    
    .......    .......    ..XXXXX    ..XXXXX
    .......    .......    .......    ..XXXXX
    .......    .......    .......    .......
    .......    .......    .......    .......
    .......    .......    .......    .......
    .......    .......    .......    .......
*/

uint32_t (R)(uint32_t first){
  return ((1 << mode_info.RedMaskSize) - 1) & 
          (first >> mode_info.RedFieldPosition);
} 

uint32_t Red(unsigned col, uint8_t step, uint32_t first) {
    return (R(first) + col * step) % (1 << mode_info.RedMaskSize);
}

uint32_t (G)(uint32_t first){
  return ((1 << mode_info.GreenMaskSize) - 1) & 
          (first >> mode_info.GreenFieldPosition);
} 

uint32_t Green(unsigned row, uint8_t step, uint32_t first) {
    return (G(first) + row * step) % (1 << mode_info.GreenMaskSize);
}

uint32_t (B)(uint32_t first){
  return ((1 << mode_info.BlueMaskSize) - 1) & 
          (first >> mode_info.BlueFieldPosition);
} 

uint32_t Blue(unsigned col, unsigned row, uint8_t step, uint32_t first) {
    return (R(first) + (col + row) * step) % (1 << mode_info.BlueMaskSize);
}

uint32_t (direct_mode)(uint32_t red, uint32_t green, uint32_t blue) {
    return (red << mode_info.RedFieldPosition) | (green << mode_info.GreenFieldPosition) | (blue << mode_info.BlueFieldPosition);
}

uint32_t (indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t no_rectangles) {
    return (first + (row * no_rectangles + col) * step) % (1 << mode_info.BitsPerPixel);
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
    if (set_frame_buffer(mode) != 0) return 1;
    if (graphic_mode(mode) != 0) return 1;

    int x = mode_info.XResolution / no_rectangles;
    int y = mode_info.YResolution / no_rectangles;

    for (int i  = 0; i < no_rectangles; i++) {
        for (int j = 0; j < no_rectangles; j++) {
            uint32_t color;
            if (mode_info.MemoryModel == 0x06) { // direct mode
                uint32_t red = Red(j, step, first);
                uint32_t green = Green(i, step, first);
                uint32_t blue = Blue(j, i, step, first);
                color = direct_mode(red, green, blue);
            } else {
                color = indexed_mode(j, i, step, first, no_rectangles);
            }
            if (vg_draw_rectangle(j * x, i * y, x, y, color)) return 1;
        }
    }
    if (kbd_test_scan() != 0) return 1;
    if (vg_exit() != 0) return 1;
    return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* To be completed */
  printf("%s(%8p, %u, %u): under construction\n", __func__, xpm, x, y);

  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}

/* aux */

int hook = KBC_IRQ;
uint8_t scancode = 0;
extern int counter;

int (kbc_get_status)(uint8_t *status) { // status do controlador do teclado
  if (status == NULL) return 1;
  if (util_sys_inb(KBC_ST_REG, status) != 0) return 1; // le status
  return 0;
}

int (kbc_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL) return 1; 
  *bit_no = BIT(hook); 
  return sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook);
}

int (kbc_unsubscribe_int)() {
  return sys_irqrmpolicy(&hook);
}

void (kbc_ih)() { // tratamento de interrupção do teclado
    uint8_t status;
    uint8_t data;
    int attempts = KBC_ATTEMPTS; // número de tentativas para ler o scancode
    while (attempts--) {
        if (kbc_get_status(&status) != 0) continue;
        if (status & KBC_OBF) { // se buffer de saida está cheio
            if (util_sys_inb(OUT_BUF, &data) != 0) continue; // lê o scancode do buffer de saída
            if ((status & (KBC_PARITY | KBC_TIMEOUT)) == 0 ) scancode = data;
            return; // retorna apos ler scancode
        }
        tickdelay(micros_to_ticks(WAIT_KBC)); // atraso para aguardar a disponibilidade do buffer de saída
    }
}

int(kbd_test_scan)() {
  uint8_t hook;
  int ipc_status;
  message msg;
  uint8_t irq_set = BIT(KBC_IRQ);
  int r;
  if (kbc_subscribe_int(&hook)) return 1;
  while (scancode != 0x81) {
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) { /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */
          if (msg.m_notify.interrupts & irq_set) { /* subscribed interrupt */
            kbc_ih(); // interrupcao teclado
          }
          break;
        default:
          break; /* no other notifications expected: do nothing */
      } 
    } else {  /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
      }
    }
	if (kbc_unsubscribe_int() != 0) return 1; // desubscreve interrupções do teclado
	return 0;
}


