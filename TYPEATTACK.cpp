#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define MAX_SENTENCES 50
#define MAX_LEN 250
#define MAX_WORDS 100
#define WORD_LEN 20


void showTypeAttackIntro(void);
void showMainMenu(int *contentMode, int *challengeMode, int *targetWPM);
int initializeDefaultFiles(void);
int loadSentences(char sentences[MAX_SENTENCES][MAX_LEN]);
void breakIntoWords(char *str, char words[MAX_WORDS][WORD_LEN], int *wordCount);
void runTypingTest(char sentences[MAX_SENTENCES][MAX_LEN], int totalSentences, int contentMode, int challengeMode, int targetWPM);
int loadHighScore(void);
void saveHighScore(int newScore);
void logGameSession(int netWPM, int rawWPM, float accuracy, int errors, int correctWords);
void viewGameHistory(void);

int main() {
   
    initializeDefaultFiles();

    char sentences[MAX_SENTENCES][MAX_LEN];
    int totalSentences = loadSentences(sentences);
    
    showTypeAttackIntro();

    int contentMode = 1;   
    int challengeMode = 1; 
    int targetWPM = 0;     

    char playAgain = 'r';
    while (playAgain == 'r' || playAgain == 'R' || playAgain == '1' || playAgain == '2') {
        showMainMenu(&contentMode, &challengeMode, &targetWPM);
        runTypingTest(sentences, totalSentences, contentMode, challengeMode, targetWPM);
        
        printf("\n+----------------------------------------+\n");
        printf("| Press 'r' to Retry, 'h' to View Logs   |\n");
        printf("| Or any other key to Exit               |\n");
        printf("+----------------------------------------+\n> ");
        
        playAgain = getchar();
        while (getchar() != '\n'); 

        if (playAgain == 'h' || playAgain == 'H') {
            viewGameHistory();
            printf("\nPress 'r' to play again or any other key to exit: ");
            playAgain = getchar();
            while (getchar() != '\n');
        }
    }

    printf("\n+----------------------------------------+\n");
    printf("| Thank you for playing TypeAttack C!    |\n");
    printf("+----------------------------------------+\n");
    return 0;
}

int initializeDefaultFiles(void) {
    FILE *sentenceFile = fopen("sentences.txt", "r");
    if (sentenceFile == NULL) {
        sentenceFile = fopen("sentences.txt", "w");
        if (sentenceFile != NULL) {
            fprintf(sentenceFile, "programming fundamentals makes learning c fun and easy\n");
            fprintf(sentenceFile, "loops and arrays are essential concepts in computer science\n");
            fprintf(sentenceFile, "practice writing code every single day to improve faster\n");
            fprintf(sentenceFile, "binary search and sorting algorithms optimize your software\n");
            fprintf(sentenceFile, "open source development builds strong technical communities\n");
            fclose(sentenceFile);
        }
    } else {
        fclose(sentenceFile);
    }

    FILE *scoreFile = fopen("highscore.txt", "r");
    if (scoreFile == NULL) {
        scoreFile = fopen("highscore.txt", "w");
        if (scoreFile != NULL) {
            fprintf(scoreFile, "0");
            fclose(scoreFile);
        }
    } else {
        fclose(scoreFile);
    }
    return 1;
}

void showTypeAttackIntro(void) {
    printf("\n");
    printf("  ----------------------------------------------------------------------------------------- \n");
    printf("                        WELCOME TO TYPEATTACK - C EDITION                                  \n");
    printf("  ----------------------------------------------------------------------------------------- \n");

}

void showMainMenu(int *contentMode, int *challengeMode, int *targetWPM) {
    int choice;
    
    printf("\n+-- SELECT CONTENT MODE ------------------+\n");
    printf("| 1. Sentences Mode (Random sentences)    |\n");
    printf("| 2. Paragraph Mode (Continuous block)    |\n");
    printf("+-----------------------------------------+\n");
    printf("Enter choice (1-2): ");
    scanf("%d", &choice);
    while (getchar() != '\n');
    
    if (choice == 2) {
        *contentMode = 2;
    } else {
        *contentMode = 1;
    }

    printf("\n+-- SELECT CHALLENGE MODE ----------------+\n");
    printf("| 1. Standard 60-Second Timed Test        |\n");
    printf("| 2. 30 WPM Target Challenge              |\n");
    printf("| 3. 60 WPM Target Challenge              |\n");
    printf("+-----------------------------------------+\n");
    printf("Enter choice (1-3): ");
    scanf("%d", &choice);
    while (getchar() != '\n');

    if (choice == 2) {
        *challengeMode = 2;
        *targetWPM = 30;
    } else if (choice == 3) {
        *challengeMode = 2;
        *targetWPM = 60;
    } else {
        *challengeMode = 1;
        *targetWPM = 0;
    }

    printf("\nPress \"Enter\" to start typing test...");
    getchar(); 
    printf("GET READY... GO!\n");
}

int loadSentences(char sentences[MAX_SENTENCES][MAX_LEN]) {
    FILE *file = fopen("sentences.txt", "r");
    if (file == NULL) {
        strcpy(sentences[0], "programming fundamentals makes learning c fun and easy");
        strcpy(sentences[1], "loops and arrays are essential concepts in computer science");
        strcpy(sentences[2], "practice writing code every single day to improve faster");
        strcpy(sentences[3], "binary search and sorting algorithms optimize your software");
        strcpy(sentences[4], "open source development builds strong technical communities");
        return 5;
    }
    
    int count = 0;
    while (count < MAX_SENTENCES && fgets(sentences[count], MAX_LEN, file) != NULL) {
        count++;
    }
    fclose(file);
    return count;
}

void breakIntoWords(char *str, char words[MAX_WORDS][WORD_LEN], int *wordCount) {
    char temp[MAX_LEN * 3];
    strcpy(temp, str);
    
    const char delimiters[] = " .;!?\t\r\n";
    char *token = strtok(temp, delimiters);
    int count = 0;

    while (token != NULL && count < MAX_WORDS) {
        strncpy(words[count], token, WORD_LEN - 1);
        words[count][WORD_LEN - 1] = '\0';
        count++;
        token = strtok(NULL, delimiters);
    }
    *wordCount = count;
}

void runTypingTest(char sentences[MAX_SENTENCES][MAX_LEN], int totalSentences, int contentMode, int challengeMode, int targetWPM) {
    int time_limit = 60; 
    time_t start_time = time(NULL);
    time_t current_time;
    
    int wordsTypedCorrectly = 0;
    int totalErrors = 0;
    int totalCharsCorrect = 0;
    int totalCharsIncorrect = 0;
    int totalCharsTyped = 0;
    int currentStreak = 0;
    int maxStreak = 0;

    srand(time(NULL));

    char largeParagraph[MAX_LEN * 3] = "programming fundamentals is an essential course for computer science students because it builds strong logic building problem solving skills and deep understanding of memory management arrays loops functions files and pointers practice daily to master writing clean and efficient code";

    while (1) {
        current_time = time(NULL);
        double elapsed_time = difftime(current_time, start_time);

        if (elapsed_time >= time_limit) {
            break;
        }

        char targetText[MAX_LEN * 3];

        if (contentMode == 2) {
            strcpy(targetText, largeParagraph);
        } else {
            int randomIndex = rand() % totalSentences;
            strcpy(targetText, sentences[randomIndex]);
            targetText[strcspn(targetText, "\r\n")] = '\0';
        }

        printf("\n+----------------------------------------+\n");
        printf("| Time Left: %.0fs  | Current Streak: %2d   |\n", time_limit - elapsed_time, currentStreak);
        printf("+----------------------------------------+\n");
        
        if (contentMode == 2) {
            printf("[Type this Paragraph]:\n");
        } else {
            printf("[Type this Sentence]:\n");
        }
        printf("%s\n", targetText);
        
        for (int i = 0; i < (strlen(targetText) > 50 ? 50 : strlen(targetText)); i++) {
            printf("-");
        }
        printf("\n> ");

        char userText[MAX_LEN * 3];
        if (fgets(userText, sizeof(userText), stdin) == NULL) {
            printf("Error reading input.\n");
            continue;
        }

        userText[strcspn(userText, "\r\n")] = '\0';
        totalCharsTyped += strlen(userText);

        char targetWords[MAX_WORDS][WORD_LEN];
        char userWords[MAX_WORDS][WORD_LEN];
        int targetCount = 0, userCount = 0;

        breakIntoWords(targetText, targetWords, &targetCount);
        breakIntoWords(userText, userWords, &userCount);

        for (int i = 0; i < targetCount; i++) {
            if (i < userCount) {
                if (strcmp(targetWords[i], userWords[i]) == 0) {
                    wordsTypedCorrectly++;
                    totalCharsCorrect += strlen(userWords[i]);
                    currentStreak++;
                    if (currentStreak > maxStreak) maxStreak = currentStreak;
                } else {
                    totalErrors++;
                    totalCharsIncorrect += strlen(userWords[i]);
                    currentStreak = 0; 
                }
            } else {
                totalErrors++; 
                currentStreak = 0;
            }
        }

        if (contentMode == 2) {
            break; 
        }
    }

    current_time = time(NULL);
    double final_elapsed = difftime(current_time, start_time);
    if (final_elapsed < 1) final_elapsed = 1;

    int netWPM = (int)(((float)wordsTypedCorrectly / final_elapsed) * 60);
    int rawWPM = (int)((((float)totalCharsTyped / 5.0f) / final_elapsed) * 60);
    float accuracy = 100.0f;
    
    float totalAttemptedWords = (float)(wordsTypedCorrectly + totalErrors);
    if (totalAttemptedWords > 0) {
        accuracy = 100.0f - (((float)totalErrors / totalAttemptedWords) * 100.0f);
        if (accuracy < 0) accuracy = 0.0f;
    }

    int highScore = loadHighScore();
    if (netWPM > highScore) {
        highScore = netWPM;
        saveHighScore(highScore);
        printf("\n*** NEW ALL-TIME HIGH SCORE RECORDED! ***\n");
    }

    logGameSession(netWPM, rawWPM, accuracy, totalErrors, wordsTypedCorrectly);

    printf("\n+---------------------------------------------------------+\n");
    printf("|                       TEST COMPLETE                     |\n");
    printf("+---------------------------------------------------------+\n");
    printf("| Net WPM (Speed): %-3d WPM      | High Score: %-3d WPM   |\n", netWPM, highScore);
    printf("| Raw WPM:         %-3d WPM      | Max Streak: %-3d words |\n", rawWPM, maxStreak);
    printf("| Accuracy:        %-6.2f%%      | Errors:     %-3d        |\n", accuracy, totalErrors);
    printf("| Time Taken:      %-5.1fs       | Correct:    %-3d words  |\n", final_elapsed, wordsTypedCorrectly);
    printf("+---------------------------------------------------------+\n");

    if (challengeMode == 2) {
        if (netWPM >= targetWPM) {
            printf(">>> CHALLENGE PASSED! You achieved %d+ WPM! <<<\n", targetWPM);
        } else {
            printf(">>> CHALLENGE FAILED! Needed %d WPM, got %d WPM. <<<\n", targetWPM, netWPM);
        }
    }
}

int loadHighScore(void) {
    FILE *file = fopen("highscore.txt", "r");
    if (file == NULL) {
        return 0;
    }
    int score = 0;
    fscanf(file, "%d", &score);
    fclose(file);
    return score;
}

void saveHighScore(int newScore) {
    FILE *file = fopen("highscore.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", newScore);
        fclose(file);
    }
}

void logGameSession(int netWPM, int rawWPM, float accuracy, int errors, int correctWords) {
    FILE *file = fopen("history.txt", "a");
    if (file != NULL) {
        time_t now = time(NULL);
        char *timeStr = ctime(&now);
        timeStr[strcspn(timeStr, "\r\n")] = '\0';

        fprintf(file, "[Date: %s] | Net WPM: %d | Raw WPM: %d | Accuracy: %.2f%% | Errors: %d | Correct: %d\n",
                timeStr, netWPM, rawWPM, accuracy, errors, correctWords);
        fclose(file);
    }
}

void viewGameHistory(void) {
    FILE *file = fopen("history.txt", "r");
    if (file == NULL) {
        printf("\nNo past game history found yet.\n");
        return;
    }

    printf("\n+-- PREVIOUS GAME SESSIONS ----------+\n");
    char line[300];
    while (fgets(line, sizeof(line), file) != NULL) {
        printf("| %s", line);
    }
    printf("+---------------------------------------------------------+\n");
    fclose(file);
}
