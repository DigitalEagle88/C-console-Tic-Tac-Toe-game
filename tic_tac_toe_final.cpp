#include <iostream>
using namespace std;
int num = 0;
// Function to check if the game ended, return 1 if player won, return -1 if computer won, 0 if nobody did
int check_end(int board[9]){
    // Make a new board format for easier checking
    int cboard[3][3] = {{board[0],board[1],board[2]},{board[3],board[4],board[5]},{board[6],board[7],board[8]}};

    // Check rows and columns
    for (int a = 0; a <= 2; a++){
        // Calculate the sum of the a'th row and column
        int row_sum = cboard[a][0]+cboard[a][1]+cboard[a][2];
        int col_sum = cboard[0][a]+cboard[1][a]+cboard[2][a];
        
        // Check if computer won
        if (row_sum == -3 || col_sum == -3){
            return(-1);
        }
        
        // Check if player won
        if (row_sum == 3 || col_sum == 3){
            return(1);
        }
    }

    // Check diagonal
    int dia_sum;
    dia_sum = cboard[0][0] + cboard[1][1] + cboard[2][2];
    
    // Check if player won
    if (dia_sum == 3){
        return (1);
    }
    //Check if computer won
    if (dia_sum == -3){
        return (-1);
    }
    
    dia_sum = cboard[0][2] + cboard[1][1] + cboard[2][0];
    
    // Check if player won
    if (dia_sum == 3){
        return (1);
    }
    // Check if computer won
    if (dia_sum == -3){
        return (-1);
    }

    // If nobody won return 0
    return (0);
}

// Function for drawing the playing board on the screen
void print_board(int board[9]){
    char vboard[9];
    
    // Create a viewable board based on the game state 
    for(int a = 0; a <= 8; a++){
        // PLayer is 'O'
        if (board[a] == 1){
            vboard[a] = 'O';
        }
        // Computer is 'X'
        else if(board[a] == -1){
            vboard[a] = 'X';
        }
        // If slot is empty, show the slot number, IDK what is this
        else{
            string b = to_string(a+1);
            char const *my_char = b.c_str();
            vboard[a] = *my_char;
        }
    }

    // Print the board
    cout << " " << vboard[0] << " | " << vboard[1] << " | " << vboard[2] << " \n";
    cout << "-----------\n";
    cout << " " << vboard[3] << " | " << vboard[4] << " | " << vboard[5] << " \n";
    cout << "-----------\n";
    cout << " " << vboard[6] << " | " << vboard[7] << " | " << vboard[8] << " \n";
}

// Function to check the results of the next possible move
int check_move(int board[9], int slot, bool is_pc, int depth, int diff){
    int stage_score[9] = {1,2,6,24,120,720,5040,40320,362880};
    int dd[5] = {1,2,3,9};
    int move_score = 0;
    num++;
    if (depth > dd[diff]){
        return 0;
    }
    
    // Copy board, fsr i can't use main board it breaks the game
    int cboard[9];
    for (int i = 0; i < 9; i++){
        cboard[i] = board[i];
    }

    // Calculate game stage
    int game_stage = 9;
    for (int i = 0; i <9; i++){
        if (cboard[i] == 0){
            game_stage--;
        }
    }

    // If is_pc is true, play as computer, else play as human
    if (is_pc){
        cboard[slot] = -1;
    }
    else {
        cboard[slot] = 1;
    }
    
    // Check if this move ends the game
    int end = 0;
    end = check_end(cboard);
    
    // Return 1 if it wins the game
    if (end == -1){
        return 1*(stage_score[9-game_stage]);
        // return 0;
        // return 1;
        // return 1*(stage_score[game_stage]);
        // return 1*game_stage^2;
    }
    
    // Return -1 if it loses the game
    if (end == 1){
        return -1*(stage_score[9-game_stage]);
        // return 0;
        // return -1;
        // return -1*(stage_score[game_stage]);
        // return -1*game_stage^2;
    }
    
    // Return 0 if it ended as draw
    if (game_stage == 9){
        return 0;
        // return 1;
        // return 1*(stage_score[9-game_stage]);
        // return 1*(stage_score[game_stage]);
    }

    // Find empty slots and check their score
    for (int i = 0; i < 9; i++){
        // Check if slot hasn't been played
        if (cboard[i] == 0){
            // If game hasn't ended yet, call self to play next move
            move_score += check_move(cboard, i, !is_pc, depth+1, diff);
        }
    }
    return move_score;
}

int chose_move(int board[9], int diff){
    int move_score[9] = {-2147483647,-2147483647,-2147483647,-2147483647,-2147483647,-2147483647,-2147483647,-2147483647,-2147483647};
    int max_score = -2147483647;
    int cboard[9];
    
    // Copy Board
    for (int i = 0; i < 9; i++){
        cboard[i] = board[i];
    }
    
    // Check scores
    for (int i = 0; i < 9; i++){
        if (cboard[i] == 0){
            move_score[i] = check_move(cboard, i, true, 1, diff);
        }
    }
    
    // Find max score
    for (int i = 0; i < 9; i++){
        if (board[i] == 0){
            if (move_score[i] >= max_score){
                max_score = move_score[i];
            }
        }
    }

    // Find the slot with max score and return its number
    for (int i = 0; i < 9; i++){
        if (move_score[i] == max_score){
            if (board[i] == 0){
                return (i);
            }
        }
    }
}

int main(){
    // Define the game board to save gamestate
    int board[9] = {0,0,0,0,0,0,0,0,0};
    int end;
    int diff = 0;

    // Store player move in an int
    int player_choice;

    // We use these to determine whether the player goes first or second
    char f;
    bool first;

    // Define an int to keep track of how many moves have been made
    int game_stage = 0;

    // Ask player if they go first and give a value to the bool 'first'
    cout << "Will you go first? y/n: ";
    cin >> f;
    if (f == 'y'){
        first = true;
    }
    else {
        first = false;
    }

    // Ask difficulty level
    cout << "PLease enter difficulty from 1 to 4: ";
    cin >> diff;
    diff = diff-1;

    // Draw the game board
    print_board(board);

    // Let player play 1 move if they chose to go first
    if (first == true){
        // Ask the player's choice
        cout << "Please enter your desired slot number:";
        cin >> player_choice;

        // Apply player choice to the board
        board[player_choice-1] = 1;

        // Add 1 to the game stage
        game_stage ++;
    }

    // Enter the main loop
    while (true)
    {
        cout << "Computer is thinking ..." << endl;

        // Determine the next move
        int computer_move = chose_move(board, diff);

        // Apply computer move to board
        board[computer_move] = -1;

        // Add 1 to the game stage
        game_stage ++;

        // Draw board
        print_board(board);
        cout << num << endl;
        // Check for win
        end = check_end(board);
        if (end == 1){
           cout << "You won!" << endl;
           main();
        }
        if (end == -1){
            cout << "you lost..." << endl;
            main();
        }
        if (game_stage == 9){ 
            cout << "It is a draw! " << endl;
            main();
        }
        
        while (true){
            // Ask the player's choice
            cout << "Please enter your desired slot number:" << endl;
            cin >> player_choice;

            // Apply player choice to the board if slot is empty
            if (board[player_choice-1] == 0){
                board[player_choice-1] = 1;
                break;
            }
            else{
                cout << "Slot already full, chose another one ..." << endl;
                continue;
            }
        }
        
        // Add 1 to the game stage
        game_stage ++;

        // Check for win
        end = check_end(board);
        if (end == 1){
           cout << "You won!" << endl;
           main();
        }
        if (end == -1){
            cout << "you lost..." << endl;
            main();
        }
        if (game_stage == 9){ 
            cout << "It is a draw! " << endl;
            main();
        }
        
    }
    return 0;
}