#include <unistd.h>
#include <termios.h>

void disable_echo() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

void enable_echo() {
    struct termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    tty.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
}

int main() {
    // Disable echo
    disable_echo();
    
    // Write to stdin
    write(STDIN_FILENO, "Hello, world!\n", 14);
    
    // Enable echo
    enable_echo();
    
    return 0;
}
