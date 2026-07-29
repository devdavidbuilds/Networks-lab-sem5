#include <stdio.h>
#include <string.h>

#define max 100
#define max_bit (max * 8)
#define max_stuff (max_bit * 2)

int data[max_bit], stuff[max_stuff], framed[max_stuff], destuff[max_bit];
char str[max];
char output_str[max];
int flag[] = {0, 1, 1, 1, 1, 1, 1, 0};
int i, b, flaglen = 8;
int bit = 0, stufflen = 0, framelen = 0; // Global trackers to maintain state between menu choices

void print(const char *label, int arr[], int n) {
    printf("%s ", label);
    for(i = 0; i < n; i++) {
        printf("%d", arr[i]);
    }
    printf("\n");
}

int main() {
    int choice;
    while(1) {
        printf("\n___ Menu ___\n");
        printf("1. Enter Data String & Generate Framed Stream\n");
        printf("2. Introduce Single Bit Error (Flip Bit)\n");
        printf("3. Destuff and Decode Framed Stream\n");
        printf("4. Manual Binary Input (Test Invalid Stuffing Error)\n");
        printf("5. Exit\n");
        printf("Enter your choice : ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input type. Exiting program.\n");
            break;
        }
        getchar(); // Clear the newline character from the buffer

        if (choice == 5) {
            printf("\nExiting>>>>\n");
            break;
        }

        int ones = 0, j = 0;
        switch(choice) {
            case 1: {
                printf("\n--- Step 1: Input & Frame Generation ---\n");
                printf("Enter a string: ");
                fgets(str, sizeof(str), stdin);
                str[strcspn(str, "\n")] = '\0';
                int str_len = strlen(str);

                // 1. String to Binary
                bit = 0;
                for(i = 0; i < str_len; i++) {
                    unsigned char ch = (unsigned char)str[i];
                    for(b = 7; b >= 0; b--) {
                        data[bit++] = (ch >> b) & 1;
                    }
                }
                print("Original data:", data, bit);

                // 2. Bit Stuffing
                j = 0;
                ones = 0;
                for(i = 0; i < bit; i++) {
                    stuff[j++] = data[i];
                    ones = (data[i] == 1) ? ones + 1 : 0;
                    if(ones == 5) {
                        stuff[j++] = 0;
                        ones = 0;
                    }
                }
                stufflen = j;
                print("Stuffed data: ", stuff, stufflen);

                // 3. Framing
                framelen = 0;
                for(i = 0; i < flaglen; i++) framed[framelen++] = flag[i];
                for(i = 0; i < stufflen; i++) framed[framelen++] = stuff[i];
                for(i = 0; i < flaglen; i++) framed[framelen++] = flag[i];

                print("Framed stream:", framed, framelen);
                break;
            }

            case 2: {
                printf("\n--- Step 2: Introduce Single Bit Error ---\n");
                if (framelen == 0) {
                    printf("Error: No framed stream exists yet. Please select Option 1 first.\n");
                    break;
                }

                int position;
                printf("Enter bit position to flip (0 to %d): ", framelen - 1);
                if (scanf("%d", &position) != 1) {
                    printf("Invalid input.\n");
                    getchar();
                    break;
                }
                getchar(); // Clear buffer

                if (position < 0 || position >= framelen) {
                    printf("Error: Out of bounds position. Must be between 0 and %d.\n", framelen - 1);
                } else {
                    // Flip the target bit
                    framed[position] = (framed[position] == 1) ? 0 : 1;
                    printf("Bit at position %d successfully flipped!\n", position);
                    print("Modified Framed stream:", framed, framelen);
                }
                break;
            }

            case 3: {
                printf("\n--- Step 3: Destuffing & Decoding ---\n");
                if (framelen == 0) {
                    printf("Error: No framed stream exists to decode. Please select Option 1 first.\n");
                    break;
                }

                ones = 0;
                j = 0;
                int error_detected = 0;

                // Perform Destuffing on the existing framed buffer
                for(i = flaglen; i < framelen - flaglen; i++) {
                    destuff[j++] = framed[i];
                    ones = (framed[i] == 1) ? ones + 1 : 0;
                    if(ones == 5) {
                        if((i + 1) >= (framelen - flaglen) || framed[i+1] != 0) {
                            printf("\n[ERROR] Invalid stuffing detected during processing!\n");
                            error_detected = 1;
                            break;
                        }
                        i++; // Skip the stuffed 0
                        ones = 0;
                    }
                }

                if (error_detected) {
                    printf("Decoding aborted due to bit corruption.\n");
                    break;
                }

                int destufflen = j;
                print("Destuffed data:", destuff, destufflen);

                // Binary Back to String
                if (destufflen % 8 != 0) {
                    printf("[WARNING] Corrupted framing payload. Bit length (%d) is not a multiple of 8.\n", destufflen);
                }

                int out_idx = 0;
                for(i = 0; i < destufflen && (i + 8) <= destufflen; i += 8) {
                    unsigned char ch = 0;
                    for(b = 0; b < 8; b++) {
                        ch = (ch << 1) | destuff[i+b];
                    }
                    output_str[out_idx++] = ch;
                }
                output_str[out_idx] = '\0';
                printf("Output decoded string: %s\n", output_str);
                break;
            }

            case 4: {
                char binary_str[max_stuff];
                int input_framed[max_stuff];
                printf("\n--- Case 4: Manual Binary Input Mode ---\n");
                printf("Enter raw binary stream to destuff: ");
                fgets(binary_str, sizeof(binary_str), stdin);
                binary_str[strcspn(binary_str, "\n")] = '\0';
                int input_len = strlen(binary_str);
                int valid_input = 1;

                for(i = 0; i < input_len; i++) {
                    if(binary_str[i] == '1') input_framed[i] = 1;
                    else if(binary_str[i] == '0') input_framed[i] = 0;
                    else {
                        printf("Error: Input must only contain 0s and 1s.\n");
                        valid_input = 0;
                        break;
                    }
                }
                if (!valid_input) break;

                printf("\nProcessing your binary stream...\n");
                print("Input stream: ", input_framed, input_len);
                ones = 0;
                j = 0;
                int discard_frame = 0;

                for(i = 0; i < input_len; i++) {
                    destuff[j++] = input_framed[i];
                    ones = (input_framed[i] == 1) ? ones + 1 : 0;
                    if(ones == 5) {
                        if(i + 1 >= input_len) {
                            printf("\n[DISCARDED] Frame error: Stream ended abruptly right after 5 consecutive 1s.\n");
                            printf("Description: HDLC protocol requires a trailing bit (either a stuffed 0 or a flag sequence) after five 1s. None was found.\n");
                            discard_frame = 1;
                            break;
                        }
                        if(input_framed[i+1] != 0) {
                            printf("\n[DISCARDED] Error: Invalid stuffing pattern detected!\n");
                            printf("Description:\n");
                            printf(" -> Found 5 consecutive '1' bits ending at index %d.\n", i);
                            printf(" -> According to bit-stuffing rules, the next bit (index %d) MUST be a stuffed '0'.\n", i + 1);
                            printf(" -> Instead, found a '%d' bit.\n", input_framed[i+1]);
                            printf(" -> DISCARDED\n");
                            discard_frame = 1;
                            break;
                        }
                        i++;
                        ones = 0;
                    }
                }
                if (discard_frame) break;

                int manual_destufflen = j;
                print("destuffed: ", destuff, manual_destufflen);
                break;
            }

            default:
                printf("Invalid selection. Please choose option 1, 2, 3, 4, or 5.\n");
                break;
        }
    }
    return 0;
}
