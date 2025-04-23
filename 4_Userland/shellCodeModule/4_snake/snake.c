#include <snake.h>
#include <stdLib.h>
#include <random.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

#define CANVAS_SIDE 768
#define SQUARE_SIDE 32
#define GRID_SIZE (CANVAS_SIDE / SQUARE_SIDE)

#define INITIAL_SNAKE_LENGTH 3

#define BLACK 0x00000000
#define RED 0x00FF4500
#define DARK_RED 0x00CC0000
#define BROWN 0x008B4513
#define GREEN 0x0032CD32
#define BLUE 0x001E90FF
#define BACKGROUND_COLOR 0x003b3b3b
#define GRID_COLOR_1 0x00CCFFCC
#define GRID_COLOR_2 0x00B2FFB2

#define BASE_TIME 150

#define INITIAL_SPACING 4

int leftBorder;

int numOfPlayers;
int gameHasStarted;

int difficulty;

char pressedKey;

typedef struct {
    int x, y;
} Position;

typedef struct {
    Position positions[GRID_SIZE * GRID_SIZE];;
    int length;
    Position direction;
    int color;
    int score;
} Snake;

Snake snake1;
Snake snake2;
Position food;

static void presentStartView();
static void startGame();
static void awaitStart();
static void play();
static void checkSnake(Snake * snake);
static void endWithMessage(char * message);
static int askReplay();
static void placeFood();
static void initializeSnakes();
static void initializeSnake(Snake * snake, int color, int offset);
static void updateSnake(Snake * snake);
static void growSnake(Snake * snake);
static int checkCollisions(Snake * snake1, Snake * snake2);
static int checkCollisionForSnake(Snake * snake);
static int checkInterSnakeCollision(Snake * snake1, Snake * snake2);
static int checkFood(Snake * snake);
static void handleKeyPresses(char c);
static void drawSnakeSection(int x, int y, int color);
static void removeSnakeSection(int x, int y);
static int askNumOfPlayers();
static int askDifficulty();
static void updatePlayerScores();
static void drawBackground();
static int getColorForCell(int x, int y);
static void drawApple(int startX, int startY);

void snake() {
    leftBorder = (WINDOW_WIDTH - CANVAS_SIDE - 1);
    
    while (1) {
        gameHasStarted = 0;
        numOfPlayers = 1;
        presentStartView();
        startGame();

        if (!askReplay()) {
            break;
        }

    }
    
    clearView();

}

static void presentStartView() {
    clearView();
    puts("Welcome to Snake game!");
    puts("Player 1 uses W/A/S/D. Player 2 uses I/J/K/L.");
    numOfPlayers = askNumOfPlayers();
    difficulty = askDifficulty();
    clearView();
}

static void startGame() {
    awaitStart();
    play();
}

static void awaitStart() {
    puts("Press SPACE to start playing...");

    while (1) {
        pressedKey = toLowercase(getchar());
        if (pressedKey == ' ') {
            if (gameHasStarted) {
                continue;
            }
            clearView();
            drawBackground();
            gameHasStarted = 1;
            initializeSnakes();
            updatePlayerScores();
            placeFood();
            break;
        }
    }
}

static void play() {
    int bufferSize;
    sleep(400);
    while(1) {

        bufferSize = 0;
        bufferSize = readInput(&pressedKey);
        pressedKey = toLowercase(pressedKey);
        if (bufferSize > 0) {
            handleKeyPresses(pressedKey);
        }

        checkSnake(&snake1);
        if (numOfPlayers == 2) {
            checkSnake(&snake2);
        }

        int collisionedSnake = checkCollisions(&snake1, &snake2);
        if (collisionedSnake && numOfPlayers == 1) {
            endWithMessage("Game Over!");
            break;
        } else if (collisionedSnake == 1) {
            endWithMessage("Player 2 Wins!");
            break;
        } else if (collisionedSnake == 2) {
            endWithMessage("Player 1 Wins!");
            break;
        }
        sleep(BASE_TIME / difficulty);
    }
}

static void checkSnake(Snake * snake) {
    updateSnake(snake);
    if (checkFood(snake)) {
        growSnake(snake);
        updateSnake(snake);
        snake->score++;
        updatePlayerScores();
        placeFood();
    }
}

static void endWithMessage(char * message) {
    clearView();
    puts(message);
    printf("Player 1: %d\n", snake1.score);
    if (numOfPlayers == 2) {
        printf("Player 2 : %d\n", snake2.score);
    }
    sleep(2000);
    clearView();
}

static int askReplay() {
    puts("Press SPACE to play again...");
    puts("Press q to quit...");

    while (1) {
        pressedKey = toLowercase(getchar());
        if (pressedKey == ' ') {
            return 1;
        } else if (pressedKey == 'q' || pressedKey == 'Q') {
            return 0;
        }
    }
}

static void placeFood() {
    int randomPosition, xPos, yPos, isOnSnake;

    do {
        isOnSnake = 0;
        randomPosition = getRandomInt(0, GRID_SIZE * GRID_SIZE - 1);
        xPos = randomPosition / GRID_SIZE;
        yPos = randomPosition % GRID_SIZE;

        for (int i = 0; i < snake1.length; i++) {
            if (snake1.positions[i].x == xPos && snake1.positions[i].y == yPos) {
                isOnSnake = 1;
                break;
            }
        }

        if (numOfPlayers == 1) {
            continue;
        }

        for (int i = 0; i < snake2.length; i++) {
            if (snake2.positions[i].x == xPos && snake2.positions[i].y == yPos) {
                isOnSnake = 1;
                break;
            }
        }

    } while (isOnSnake);

    food.x = xPos;
    food.y = yPos;
    drawApple(leftBorder + xPos * SQUARE_SIDE, yPos * SQUARE_SIDE);
}

static void initializeSnakes() {
    initializeSnake(&snake1, GREEN, -(INITIAL_SPACING / 2));
    if (numOfPlayers == 1) {
        return;
    }
    initializeSnake(&snake2, BLUE, INITIAL_SPACING / 2);
}

static void initializeSnake(Snake * snake, int color, int offset) {
    snake->length = INITIAL_SNAKE_LENGTH;
    snake->direction.x = 1;
    snake->direction.y = 0;
    snake->color = color;
    snake->score = 0;
    for (int i = 0; i < snake->length; i++) {
        snake->positions[i].x = GRID_SIZE / 2 - i;
        snake->positions[i].y = GRID_SIZE / 2 - offset;
        drawSnakeSection(snake->positions[i].x, snake->positions[i].y, snake->color);
    }
}

static void updateSnake(Snake * snake) {
    removeSnakeSection(snake->positions[snake->length-1].x, snake->positions[snake->length-1].y);
    for (int i = snake->length - 1; i > 0; i--) {
        snake->positions[i] = snake->positions[i-1];
    }
    snake->positions[0].x += snake->direction.x;
    snake->positions[0].y += snake->direction.y;
    drawSnakeSection(snake->positions[0].x, snake->positions[0].y, snake->color);
}

static void growSnake(Snake * snake) {
    snake->length++;
    snake->positions[snake->length - 1] = snake->positions[snake->length - 2];
}

static int checkCollisions(Snake * snake1, Snake * snake2) {
    if (checkCollisionForSnake(snake1)) {
        return 1;
    }
    if (numOfPlayers == 1) {
        return 0;
    }
    if (checkCollisionForSnake(snake2)) {
        return 2;
    }
    if (checkInterSnakeCollision(snake1, snake2)) {
        return 1;
    }
    if (checkInterSnakeCollision(snake2, snake1)) {
        return 2;
    }
    return 0;
}

static int checkCollisionForSnake(Snake * snake) {
    if (snake->positions[0].x < 0 || snake->positions[0].x >= GRID_SIZE || 
        snake->positions[0].y < 0 || snake->positions[0].y >= GRID_SIZE) {
            return 1;
    }
    for (int i = 1; i < snake->length; i++) {
        if (snake->positions[0].x == snake->positions[i].x && 
            snake->positions[0].y == snake->positions[i].y) {
            return 1;
        }
    }
    return 0;
}

static int checkInterSnakeCollision(Snake * snake1, Snake * snake2) {
    for (int i = 0; i < snake2->length; i++) {
        if (snake1->positions[0].x == snake2->positions[i].x && 
        snake1->positions[0].y == snake2->positions[i].y) {
            return 1;
        }
    }
    return 0;
}

static int checkFood(Snake * snake) {
    return (snake->positions[0].x == food.x && snake->positions[0].y == food.y);
}

static void handleKeyPresses(char c) {
    switch (c) {
        case 'w':
            if (snake1.direction.y == 1) {
                break;
            }
            snake1.direction.x = 0;
            snake1.direction.y = -1;
            break;
        case 'a':
            if (snake1.direction.x == 1) {
                break;
            }
            snake1.direction.x = -1;
            snake1.direction.y = 0;
            break;
        case 's':
            if (snake1.direction.y == -1) {
                break;
            }
            snake1.direction.x = 0;
            snake1.direction.y = 1;
            break;
        case 'd':
            if (snake1.direction.x == -1) {
                break;
            }
            snake1.direction.x = 1;
            snake1.direction.y = 0;
            break;
    }

    if (numOfPlayers == 1) {
        return;
    }

    switch(c) {
        case 'i':
            if (snake2.direction.y == 1) {
                break;
            }
            snake2.direction.x = 0;
            snake2.direction.y = -1;
            break;
        case 'j':
            if (snake2.direction.x == 1) {
                break;
            }
            snake2.direction.x = -1;
            snake2.direction.y = 0;
            break;
        case 'k':
            if (snake2.direction.y == -1) {
                break;
            }
            snake2.direction.x = 0;
            snake2.direction.y = 1;
            break;
        case 'l':
            if (snake2.direction.x == -1) {
                break;
            }
            snake2.direction.x = 1;
            snake2.direction.y = 0;
    }

}

static void drawSnakeSection(int x, int y, int color) {
    drawRectangle(leftBorder + x * SQUARE_SIDE, y * SQUARE_SIDE, SQUARE_SIDE, SQUARE_SIDE, color);
}

static void removeSnakeSection(int x, int y) {
    drawRectangle(leftBorder + x * SQUARE_SIDE, y * SQUARE_SIDE, SQUARE_SIDE, SQUARE_SIDE, getColorForCell(x, y));
}

static int askNumOfPlayers() {
    char buffer[2];
    int numOfPlayers;
    do {
        putsNoNewLine("Enter the number of players (1 / 2): ");
        gets(buffer, 2);
        numOfPlayers = stringToInt(buffer);
    } while (numOfPlayers != 1 && numOfPlayers != 2);
    return numOfPlayers;
}

static int askDifficulty() {
    char buffer[2];
    int difficulty;
    do {
        putsNoNewLine("Enter the difficulty (1 - 5): ");
        gets(buffer, 2);
        difficulty = stringToInt(buffer);
    } while (difficulty < 1 || difficulty > 5);
    return difficulty;
}

static void updatePlayerScores() {
    resetCursor();
    drawRectangle(0, 0, 200, 200, BLACK);
    printf("Player 1: %d\n", snake1.score);
    if (numOfPlayers == 2) {
        printf("Player 2: %d\n", snake2.score);
    }
}

static void drawBackground() {
    int backgroundColor;
    if (gameHasStarted) {
        return;
    }
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            if ((i + j) % 2 == 0) {
                backgroundColor = GRID_COLOR_1;
            } else {
                backgroundColor = GRID_COLOR_2;
            }
            drawRectangle(leftBorder + i * SQUARE_SIDE, j * SQUARE_SIDE, SQUARE_SIDE, SQUARE_SIDE, backgroundColor);
        }
    }
}

static int getColorForCell(int x, int y) {
    return ((x + y) % 2 == 0) ? GRID_COLOR_1 : GRID_COLOR_2;
}

void drawApple(int startX, int startY) {
    for (uint64_t y = 2; y < 6; y++) {
        for (uint64_t x = 11; x < 18; x++) {
            if ((y == 2 && (x < 13 || x > 15)) || (y == 5 && (x < 12 || x > 16))) continue;
            sys_draw_pixel(startX + x, startY + y, GREEN);
        }
    }

    for (uint64_t y = 6; y < 9; y++) {
        for (uint64_t x = 14; x < 18; x++) {
            sys_draw_pixel(startX + x, startY + y, BROWN);
        }
    }

    for (uint64_t y = 8; y < 28; y++) {
        for (uint64_t x = 5; x < 27; x++) {
            if ((x - 16) * (x - 16) + (y - 18) * (y - 18) <= 100) {
                if ((x - 16) * (x - 16) + (y - 20) * (y - 20) > 80) {
                    sys_draw_pixel(startX + x, startY + y, DARK_RED);
                } else {
                    sys_draw_pixel(startX + x, startY + y, RED);
                }
            }
        }
    }
}
