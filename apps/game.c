#include "kernel.h"
#include "drivers.h"
#include "oled.h"
#include "stm32f4xx.h" 

// Simple Pong-like game
typedef struct {
    int ball_x, ball_y;
    int ball_dx, ball_dy;
    int paddle1_y, paddle2_y;
    int score1, score2;
    int paddle_height;
} pong_game_t;

static pong_game_t game;

void game_init(void) {
    oled_init();
    
    // Initialize game state
    game.ball_x = OLED_WIDTH / 2;
    game.ball_y = OLED_HEIGHT / 2;
    game.ball_dx = 1;
    game.ball_dy = 1;
    game.paddle1_y = OLED_HEIGHT / 2 - 10;
    game.paddle2_y = OLED_HEIGHT / 2 - 10;
    game.paddle_height = 20;
    game.score1 = 0;
    game.score2 = 0;
}

void game_update_physics(void) {
    // Move ball
    game.ball_x += game.ball_dx;
    game.ball_y += game.ball_dy;
    
    // Ball collision with top/bottom
    if (game.ball_y <= 0 || game.ball_y >= OLED_HEIGHT - 1) {
        game.ball_dy = -game.ball_dy;
    }
    
    // Ball collision with paddles
    if (game.ball_x <= 2) {
        if (game.ball_y >= game.paddle1_y && game.ball_y <= game.paddle1_y + game.paddle_height) {
            game.ball_dx = -game.ball_dx;
            // Increase speed slightly
            if (game.ball_dx > 0) game.ball_dx++;
            else game.ball_dx--;
        }
    }
    
    if (game.ball_x >= OLED_WIDTH - 3) {
        if (game.ball_y >= game.paddle2_y && game.ball_y <= game.paddle2_y + game.paddle_height) {
            game.ball_dx = -game.ball_dx;
            // Increase speed slightly
            if (game.ball_dx > 0) game.ball_dx++;
            else game.ball_dx--;
        }
    }
    
    // Score points
    if (game.ball_x < 0) {
        game.score2++;
        game_reset_ball();
    }
    
    if (game.ball_x >= OLED_WIDTH) {
        game.score1++;
        game_reset_ball();
    }
}

void game_reset_ball(void) {
    game.ball_x = OLED_WIDTH / 2;
    game.ball_y = OLED_HEIGHT / 2;
    game.ball_dx = (game.ball_dx > 0) ? 1 : -1;
    game.ball_dy = (rand() % 2) ? 1 : -1;
}

void game_update_paddles(void) {
    // Simple AI for paddle2 (follows ball)
    if (game.paddle2_y + game.paddle_height/2 < game.ball_y) {
        game.paddle2_y++;
    } else if (game.paddle2_y + game.paddle_height/2 > game.ball_y) {
        game.paddle2_y--;
    }
    
    // Keep paddles in bounds
    if (game.paddle1_y < 0) game.paddle1_y = 0;
    if (game.paddle1_y > OLED_HEIGHT - game.paddle_height) game.paddle1_y = OLED_HEIGHT - game.paddle_height;
    if (game.paddle2_y < 0) game.paddle2_y = 0;
    if (game.paddle2_y > OLED_HEIGHT - game.paddle_height) game.paddle2_y = OLED_HEIGHT - game.paddle_height;
}

void game_render(void) {
    oled_clear();
    
    // Draw ball
    oled_draw_rect(game.ball_x, game.ball_y, 2, 2, true, true);
    
    // Draw paddles
    oled_draw_rect(0, game.paddle1_y, 2, game.paddle_height, true, true);
    oled_draw_rect(OLED_WIDTH - 2, game.paddle2_y, 2, game.paddle_height, true, true);
    
    // Draw center line
    for (int y = 0; y < OLED_HEIGHT; y += 4) {
        oled_draw_pixel(OLED_WIDTH / 2, y, true);
    }
    
    // Draw scores (simplified)
    oled_draw_rect(OLED_WIDTH/4 - 5, 2, 3, 5, true, true); // Player 1 score indicator
    oled_draw_rect(3*OLED_WIDTH/4 - 5, 2, 3, 5, true, true); // Player 2 score indicator
    
    oled_update();
}

void game_handle_input(void) {
    // Read GPIO buttons for paddle control
    static int last_button1 = 0;
    static int last_button2 = 0;
    
    int button1 = gpio_read(0); // Button for paddle1 up
    int button2 = gpio_read(1); // Button for paddle1 down
    
    if (button1 && !last_button1) {
        game.paddle1_y -= 3;
    }
    if (button2 && !last_button2) {
        game.paddle1_y += 3;
    }
    
    last_button1 = button1;
    last_button2 = button2;
}

void game_task(void *arg) {
    game_init();
    
    uart_puts("Pong Game Started!\r\n");
    uart_puts("Controls: Button1=Up, Button2=Down\r\n");
    
    while (1) {
        game_handle_input();
        game_update_physics();
        game_update_paddles();
        game_render();
        
        // Game speed control
        for (int i = 0; i < 50; i++) {
            task_yield();
        }
        
        // Print score occasionally
        static uint32_t frame_count = 0;
        if (frame_count++ % 100 == 0) {
            uart_puts("Score: ");
            // uart_putn(game.score1);
            uart_puts(" - ");
            // uart_putn(game.score2);
            uart_puts("\r\n");
        }
    }
}

void game_init_task(void) {
    // Initialize GPIO for buttons
    gpio_set_direction(0, 0); // Button 1 input
    gpio_set_direction(1, 0); // Button 2 input
    
    create_task(game_task, NULL, 4);
}

// Utility function for number printing (add to uart.c)
void uart_putn(uint32_t num) {
    char buffer[10];
    int i = 0;
    
    if (num == 0) {
        uart_putchar('0');
        return;
    }
    
    while (num > 0) {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }
    
    while (--i >= 0) {
        uart_putchar(buffer[i]);
    }
}
