#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_DOCS 100
#define MAX_LEN 200

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + 32;
        }
    }
}

char documents[MAX_DOCS][MAX_LEN];
int doc_count = 0;

int isDuplicate(char newDoc[]) {
    for (int i = 0; i < doc_count; i++) {
        if (strcmp(documents[i], newDoc) == 0) {
            return 1; // duplicate found
        }
    }
    return 0; // unique
}

void addDocuments() {
    char input[50];
    char *endptr;
    int n;

    printf("Enter number of documents: ");
    fgets(input, sizeof(input), stdin);

    n = strtol(input, &endptr, 10);

    // Check if invalid input
    if (endptr == input || *endptr != '\n') {
        printf("Invalid number! Please enter a valid integer.\n");
        return;
    }

    // Range validation
    if (n <= 0 || n + doc_count > MAX_DOCS) {
        printf("Invalid range! Enter between 1 and %d\n", MAX_DOCS - doc_count);
        return;
    }

    for (int i = 0; i < n; i++) {
    	char temp[MAX_LEN];

    	printf("Enter document %d: ", doc_count + 1);
    	fgets(temp, MAX_LEN, stdin);

    	temp[strcspn(temp, "\n")] = '\0';

    	// Check duplicate
    	if (isDuplicate(temp)) {
        	printf("Duplicate entry! Document not added.\n");
        	i--; // retry same index
        	continue;
    	}

    	strcpy(documents[doc_count], temp);
    	doc_count++;
	}
}

// Function to display documents
void displayDocuments() {
    printf("\nStored Documents:\n");
    for (int i = 0; i < doc_count; i++) {
        printf("Doc %d: %s\n", i + 1, documents[i]);
    }
}

int countOccurrences(char *text, char *keyword) {
    int count = 0;
    char *pos = text;

    while ((pos = strstr(pos, keyword)) != NULL) {
        count++;
        pos += strlen(keyword); // move forward
    }

    return count;
}

void searchDocuments() {
    char keyword[50];

    printf("Enter keyword to search: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = '\0';

    // Validate empty input
    if (strlen(keyword) == 0) {
        printf("Invalid keyword!\n");
        return;
    }

    // Convert keyword to lowercase
    char keywordLower[50];
    strcpy(keywordLower, keyword);
    toLowerCase(keywordLower);

    int freq[MAX_DOCS];
    int index[MAX_DOCS];

    // Calculate frequency
    for (int i = 0; i < doc_count; i++) {
        char tempDoc[MAX_LEN];
        strcpy(tempDoc, documents[i]);
        toLowerCase(tempDoc);

        freq[i] = countOccurrences(tempDoc, keywordLower);
        index[i] = i; // store original index
    }

    // Sort using index (not modifying original data)
    for (int i = 0; i < doc_count - 1; i++) {
        for (int j = i + 1; j < doc_count; j++) {
            if (freq[j] > freq[i]) {
                // swap freq
                int temp = freq[i];
                freq[i] = freq[j];
                freq[j] = temp;

                // swap index
                int tempIdx = index[i];
                index[i] = index[j];
                index[j] = tempIdx;
            }
        }
    }

    // Display results
    int found = 0;
    printf("\nSearch Results (Ranked):\n");

    for (int i = 0; i < doc_count; i++) {
        if (freq[i] > 0) {
            printf("Doc %d (Score: %d): %s\n",
                   index[i] + 1,
                   freq[i],
                   documents[index[i]]);
            found = 1;
        }
    }

    if (!found) {
        printf("No matching documents found.\n");
    }
}

int main() {
    int choice;

    while (1) {
        printf("\n--- Search Engine Menu ---\n");
        printf("1. Add Documents\n");
        printf("2. Display Documents\n");
        printf("3. Search Documents\n");  
	    printf("4. Exit\n");              
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
    		case 1:
        		addDocuments();
        		break;

    		case 2:
        		displayDocuments();
        		break;

    		case 3:
        		searchDocuments();
        		break;

    		case 4:
        		return 0;

    		default:
        		printf("Invalid choice!\n");
}
    }

    return 0;
}
