# Reversi-Bot

Playable terminal based reversi/othello game created as a project for APS105 at the University of Toronto. This bot was ranked number one for the course in 2023.

This bot utilizes alpha-beta pruning on the Minimax algorithm. The constraints for this project was the decision making time for the bot had to be less than one second.

The heurisitics used for the game engine were:
- Score parity
- Corners
- Mobility
- Stability

The weighting of each heurisitc was dynamically changed based on the phase of the game. i.e. mobility and corners was favoured more in the opening stages of the game whereas score parity was favoured in the end game. 
