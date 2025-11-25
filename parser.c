#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define MAX_LINE_LENGTH 1024
#define PASSWD_FILE "/etc/passwd"

// Prototypes
void print_str(const char *str);
int parse_passwd_file(const char *filePath);
void extract_user_and_shell(char *line);

int main() {
    int result = parse_passwd_file(PASSWD_FILE);
    
    if (result != 0) {
        if (result == 1) print_str("Error: Failed to open file.\n");
        if (result == 2) print_str("Error: Read failure occurred.\n");
        return result;
    }
    return 0;
}

// Custom implementation of print using write() syscall to avoid stdio overhead
void print_str(const char *str) {
    if (str) {
        write(STDOUT_FILENO, str, strlen(str));
    }
}

// Parses a line in place to find username (field 1) and shell (field 7)
void extract_user_and_shell(char *line) {
    char *username = line;
    char *shell = NULL;
    int field = 0; 

    // Iterate through the line, replacing ':' with null terminators to split fields
    for (int i = 0; line[i] != '\0'; i++) {
        if (line[i] == ':') {
            field++;
            line[i] = '\0'; // Null terminate extracted field
            
            // Field 6 (0-indexed) is the 7th field, which is usually the shell path
            if (field == 6) { 
                shell = &line[i+1];
                break; // Found the shell, stop parsing
            }
        }
    }

    if (username && shell) {
        print_str(username);
        print_str(" : ");
        print_str(shell);
        print_str("\n");
    }
}

int parse_passwd_file(const char *filePath) {
    int fd = open(filePath, O_RDONLY);
    if (fd == -1) {
        return 1; 
    }

    char buffer[MAX_LINE_LENGTH];
    char line_buffer[MAX_LINE_LENGTH];
    int line_idx = 0;
    ssize_t bytes_read;

    // Read file in chunks
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                // End of line detected, extract data
                line_buffer[line_idx] = '\0';
                extract_user_and_shell(line_buffer);
                line_idx = 0; // Reset buffer for next line
            } else {
                if (line_idx < MAX_LINE_LENGTH - 1) {
                    line_buffer[line_idx++] = buffer[i];
                }
            }
        }
    }

    if (bytes_read == -1) {
        close(fd);
        return 2;
    }

    close(fd);
    return 0;
}
