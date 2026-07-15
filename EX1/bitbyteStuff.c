#include <stdio.h>
#include <string.h>

void bitStuffing()
{
    char data[100], result[200];
    int i, j = 0, count = 0;

    printf("Enter binary data: ");
    scanf("%s", data);

    for (i = 0; data[i] != '\0'; i++)
    {
        result[j++] = data[i];

        if (data[i] == '1')
        {
            count++;
            if (count == 5)
            {
                result[j++] = '0';
                count = 0;
            }
        }
        else
        {
            count = 0;
        }
    }

    result[j] = '\0';
    printf("Bit Stuffed Data: %s\n", result);
}

void bitDestuffing()
{
    char data[200], result[200];
    int i, j = 0, count = 0;

    printf("Enter stuffed binary data: ");
    scanf("%s", data);

    for (i = 0; data[i] != '\0'; i++)
    {
        result[j++] = data[i];

        if (data[i] == '1')
        {
            count++;

            if (count == 5)
            {
                i++;      // Skip stuffed 0
                count = 0;
            }
        }
        else
        {
            count = 0;
        }
    }

    result[j] = '\0';
    printf("Bit Destuffed Data: %s\n", result);
}

void byteStuffing()
{
    char data[100], result[200];
    char FLAG = 'F';
    char ESC = 'E';
    int i, j = 0;

    printf("Enter data: ");
    scanf("%s", data);

    result[j++] = FLAG;

    for (i = 0; data[i] != '\0'; i++)
    {
        if (data[i] == FLAG || data[i] == ESC)
            result[j++] = ESC;

        result[j++] = data[i];
    }

    result[j++] = FLAG;
    result[j] = '\0';

    printf("Byte Stuffed Data: %s\n", result);
}

void byteDestuffing()
{
    char data[200], result[200];
    char FLAG = 'F';
    char ESC = 'E';
    int i, j = 0;

    printf("Enter stuffed data: ");
    scanf("%s", data);

    for (i = 1; data[i] != FLAG && data[i] != '\0'; i++)
    {
        if (data[i] == ESC)
            i++;

        result[j++] = data[i];
    }

    result[j] = '\0';

    printf("Byte Destuffed Data: %s\n", result);
}

int main()
{
    int choice;

    do
    {
        printf("\n===== MENU =====\n");
        printf("1. Bit Stuffing\n");
        printf("2. Bit Destuffing\n");
        printf("3. Byte Stuffing\n");
        printf("4. Byte Destuffing\n");
        printf("5. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice)
        {
            case 1:
                bitStuffing();
                break;

            case 2:
                bitDestuffing();
                break;

            case 3:
                byteStuffing();
                break;

            case 4:
                byteDestuffing();
                break;

            case 5:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid Choice!\n");
        }

    } while(choice != 5);

    return 0;
}
