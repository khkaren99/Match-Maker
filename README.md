# Match-Maker
A GUI tool named “Match Maker” that organizes game matches for players for various games, shows their ratings for each game on real-time dashboard, and stores all that data in the disc so it can be retrieved any time the tool is launched.

***

## Definitions of terms:
Player/User – it’s just a bot that plays any of the games randomly. All players are stored in a database (or any type of regular file) of your preference. A player consists of Username, First Name, Last Name, and Preferred Games.

Username – an ASCII string with no invisible characters and with length not greater than 16.

First Name – a string that consists of Latin characters only and starts with a capital letter.

Last Name – a string that consists of Latin characters and also can contain a single quote mark in the middle (used for last names like O’Hara or d’Cartes).

Preferred Games – a list of games that the player prefers to play. It can be a list of game executable paths.

Game – is a separate non-GUI program. It accepts two usernames as an input. When the game finishes, it outputs the username of the winner. If it’s a draw, it outputs usernames of both players.

Rating – for each game, each player has their rating which is the number of matches won. Initial rating of all players is in all games is 0.


## Description of a game:
Minimum number of games is 3. You can add any number of games of your choice.

All games are turn based.

Tic-Tac-Toe is an example of such game. You can implement various variants of it, like 3x3, 4x4, 5x5, etc.

You can also implement other turn based simple games that can be played randomly.

E.g., Rock-Paper-Scissors, Dice, etc.


Each game should take a random time between 0.5 to 2 seconds (not strict bounds) due to each player “thinking” time.


## Matchmaking:
Each player at any point of time can be in 3 possible states: Free, Busy, Waiting for match.

Each player’s initial state is “Free”. The player after being “Free” for 2 seconds sends a match request to play a random game from their preferred list to match making engine and waits for some time randomly between 2-3 seconds. Within that interval their state changes to “Waiting for match”.

Within that time the match maker should provide an opponent a rating of which is no greater than 3 or no lower than 1 of the requester’s rating. If no match is found the player goes back to “Free” state.

When the game starts, both players go to “Busy” state.

After the game is finished, they go to “Free” state.


## Main Window:
The tool’s main window consists of two menus: “Dashboard” and “Users List” (see “MainWindow.png”).

![0](https://github.com/user-attachments/assets/de4e8a38-5dd8-4eef-bf75-a0808982aa65)

Users List – a table widget that contains the user data. Users can be added or removed from that table but cannot be modified. The table can be sorted by first 3 columns when the corresponding header is clicked.

Dashboard – shows the ratings of each player for each game in a form of a tree with depth of 2. Each root represents a single game.

Add user dialog – a separate dialog used to add a player (see “AddUserDialog.png”). Appears when “+Add User” is clicked in the table or from context menu (details below) or from toolbar (details below).

![AddUserDialog](https://github.com/user-attachments/assets/302c6d08-41a3-4a16-a2ae-c379ba1055bd)

Users can be removed using context menu (details below) or from toolbar (details below).

User Filter – used to filter out the users in the above table Usernames of which match the specified regexp.

Each time the tool is launched it should show the latest Dashboard and User List (i.e., it should be saved when the tool is closed).

Context menus (i.e., the menus that are invoked by right mouse button click):

Users List context menu – consists of “Add User” and “Remove User” actions (see “AddRemoveUserContextMenu.png”).

![AddRemoveUserContextMenu-1](https://github.com/user-attachments/assets/835bb44b-0aac-4df5-99dd-0742dfb91218)

Dashboard context menu – consists of “Save to File” action and only applicable when invoked on game element (see “SaveToFileContextMenu.png”). Saves the ratings for the selected game to a text file.

![SaveToFileContextMenu](https://github.com/user-attachments/assets/1f035caf-1ba8-4b2c-8386-a36025b18bca)



## Toolbar:
File->Save the Dashboard to File – saves the content of the Dashboard to a text file (see “FileMenu.png”).

![FileMenu](https://github.com/user-attachments/assets/640a3344-854c-47a8-a1c1-e971bc5cef49)

Edit->+Add User – adds user to Users List (see “EditMenu.png”).

Edit->-Remove User – removes user from the Users List (see “EditMenu.png”).

![EditMenu](https://github.com/user-attachments/assets/7e691506-dc4e-4dc4-885b-a1000a9a3f2a)

View->Show/Hide Dashboard – toggles Dashboard’s visibility (see “ViewMenu.png”).

View->Show/Hide User List – toggles User List’s visibility (see “ViewMenu.png”).

![ViewMenu](https://github.com/user-attachments/assets/bafd4f85-893f-4b30-ab5a-862580f98f29)


## Platforms
Linux


## Limitation
C++ standard – 17

Qt version – 5


## Usage
Build: run ./build.sh

Run app: ./build/bin/app

Run game: ./build/bin/game gameName userName1 userName2
