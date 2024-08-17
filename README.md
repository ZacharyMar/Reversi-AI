# Reversi-AI

Playable terminal-based reversi/Othello game created as a project for APS105 at the University of Toronto. This bot was ranked number one for the course in 2023.

This bot utilizes alpha-beta pruning on the Minimax algorithm. The AI has a maximum decision-making time of one second.

The heuristics used for the game engine were:
- Score parity
- Corners
- Mobility
- Stability

The weighting of each heuristic was dynamically changed based on the phase of the game. i.e. mobility and corners were favoured more in the opening stages of the game whereas score parity was favoured in the end game. 
