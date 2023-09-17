**Connect4**


In this project, we have studied various algorithms on the same, and made a quantitative analysis of their working, and compared the various algorithms used for the analysis.

**Problem Statement**

To study various algorithms for Connect-4 (expandable to Connect-X) and make a comparative analysis of the same, on the basis of various attributes (Depths explored, Nodes explored, and execution time in choosing a certain move).

**Methodology & Demonstration**

-   Bitboard
-   Negamax
-   Alpha beta pruning with Negamax
-   Principal Variation with Alpha Beta pruning
-   Iterative Deepening
-   Heuristic

    The details of the above terminologies and/or algorithms are given in detail below.


**Study / Experiment settings**

The comparative analysis was done in various stages.

**Bitboards -** First stage was creating the game, where we used bitboards instead of arrays (matrices) for efficient and effective use of space. We used two bitboards, one that represented the main board and the other that represented the current players pieces. Additionally, a mask bitboard is used for implementing utilities within the board. The 1st bitboard represents cells occupied by the current player and 2nd bitboard represents cells occupied in the whole board (performing bit masking can easily switch player boards and implement other utilities). Since our state space is 6x7 for a standard connect-4 game we require only 1 int of 64 bit to represent a single bitboard. It also helps in easy understanding and faster accessing of the states of the board/game.

**Negamax** - This algorithm works on the same game trees as those used by the minimax algorithm. The main difference between the two is that in Minimax, we have 2 functions, namely the maximizer and the minimizer, which try to retrieve the maximum and the minimum scores respectively, whereas in Negamax, we apply the logic that minimum of the scores retrieved is basically the maximum of those values with inverted signs i.e. **max(a,b) = - min(-a,-b)** , so minimizer is replaced and we have a single function to approach the problem.


**Alpha beta pruning -**  This algorithm seeks to decrease the number of nodes that are evaluated by the minimax algorithm (can be used for Negamax too, since Negamax is derived from minimax itself). It stops evaluating a move when at least one possibility has been found that proves the move to be worse than a previously examined move because such moves need not be evaluated further.


**Principal Variation -** This algorithm is usually used in extension to the alpha beta pruning and it further gives a performance improvement to the algorithm used.

The logic this algorithm uses is once we have identified a move with a score between alpha and beta, we search the remaining movements to show that they are all poor. This can be accomplished a little bit more quickly than a search that is concerned that one of the remaining moves might be advantageous. If the above mentioned condition holds false, we then continue with the basic alpha beta pruning algorithm.

**Heuristic -** The heuristic used in the project works such that we have chosen all the value ranging from **n/2 to n-1** (n is the target value), and we choose the factor as (**n+1)\*(2\^i),** i starting from 0**,** where i increases by 1 in subsequent values as shown in the example.


The final stage we generated the functions to export execution results for ai move selections for generating visualizations and drawing comparisons. Results included move chosen, score for selected move, nodes explored, execution time and depths explored.

**Results (along with comparison with baseline model(s))**

To determine any first player advantage we chose AI vs AI for different board dimensions with target four and the results are in the table below. The maxDepths were set to total no. of moves so that heuristic is never called and evaluations are purely made by win/tie/lose.

|             | **Height 4**  | **Height 5** | **Height 6**  |
|-------------|---------------|--------------|---------------|
| **Width 4** | Draw          | Draw         | Draw          |
| **Width 5** | Draw          | Draw         | Draw          |
| **Width 6** | Second Player | Draw         | Second Player |
| **Width 7** | Draw          | Draw         | First Player  |

In the standard version (highlighted in blue) of the game it is advantageous to play the first move or the game is biased in favor of the first player iff both players play their best. However it's advantageous for the second player for certain board dimensions while drawing in most of the cases.

Algorithms vs time taken by them to complete an AI vs AI game in standard connect-4 game.

| **ALGORITHM**                                   | **TOTAL TIME(ms)** | **AVG TIME(ms)** |
|-------------------------------------------------|--------------------|------------------|
| **Negamax**                                     | 2860.22            | 73.33            |
| **Negamax with Alpha Beta Pruning**             | 94.53              | 2.42             |
| **Principal Variation with Alpha Beta Pruning** | 46.48              | 1.19             |

Clearly with Negamax our AI takes much more time compared to that of pruning and principal variation. The difference was evident while testing to the naked eye that Negamax would take a large amount of time to complete the game while other two algorithms completed the game within a few seconds.

Iterative deepening although takes more time (because Iterative deepening is cut off using time boundations), but aides in dynamic depth searches.

| **ALGORITHM WITH ITERATIVE DEEPENING**          | **TOTAL TIME(ms)** | **AVG TIME(ms)** |
|-------------------------------------------------|--------------------|------------------|
| **Negamax**                                     | 48153.64           | 2675.2           |
| **Negamax with Alpha Beta Pruning**             | 48002.3            | 1411.83          |
| **Principal Variation with Alpha Beta Pruning** | 35385.73           | 1179.52          |


| **ALGORITHM**                                   | **NUMBER OF MOVES WITHOUT ITERATIVE DEEPENING** | **NUMBER OF MOVES WITH** **ITERATIVE** **DEEPENING** |
|-------------------------------------------------|-------------------------------------------------|------------------------------------------------------|
| **Negamax**                                     | 39                                              | 18                                                   |
| **Negamax with Alpha Beta Pruning**             | 39                                              | 35                                                   |
| **Principal Variation with Alpha Beta Pruning** | 39                                              | 31                                                   |


**Conclusions with limitations and an outlook on future work**

The conclusions that can be drawn are as follows:

1.  Total execution time: Negamax \> alpha_beta pruning \> principal variation search.

=\> In equal time max_depths reached: principal variation search \> alpha_beta pruning \> Negamax.

1.  No. of nodes explored: Negamax \> alpha_beta pruning \> principal variation.

    =\> No. of less useful moves pruned: principal variation search \> alpha_beta pruning \> Negamax.

2.  Iterative deepening comparisons show that depths are dynamically handled and games are finished earlier.

    =\> Better moves can be found and evaluation will be more accurate and the game can be possibly finished in fewer moves.

    The limitation to the current scenario is that the (rows + 1) \* columns cannot exceed 64 since we are using a 64 bit integer value to represent the complete board. This is something that can be looked upon to improve the project in the future by using multiple integers in continuation and handling the bit management between the first and last bits of the 2 numbers respectively. Using certain new algorithms like Monte Carlo Tree Search, or using Reinforcement Learning methods may also help in increasing efficiency of move selections.

    **References**

    <https://en.wikipedia.org/wiki/Principal_variation_search>

    <https://en.wikipedia.org/wiki/Negamax>

    [https://en.wikipedia.org/wiki/Alpha–beta_pruning](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning)

    <https://web.archive.org/web/20071026090003/http://www.brucemo.com/compchess/programming/index.htm>

