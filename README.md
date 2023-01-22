# 421-Solver

Solving [421 game](https://fr.wikipedia.org/wiki/421_(jeu)) one chip heads-up variant with minmax algorithm using Markov decision process, dynamic programming and symmetry breaking  

## Mathematical Model

Let **X<sub>ra</sub>** be the discrete random variable of a dice throw given optional : prior combination **r** and action **a**.  

Every function is computed from the starting player's perspective.  

Let **eval(r<sub>1</sub>, r<sub>2</sub>)** return 1 if combination **r<sub>1</sub>** has a better ranking than **r<sub>2</sub>**, 0.5 if tie and 0 otherwise.  

Let **g(r<sub>1</sub>, r<sub>2</sub>, t)** be the winning odds for a fixed combination **r<sub>1</sub>** against **r<sub>2</sub>** with **t** rerolls left to use.  

Let **f(r<sub>1</sub>, t)** be the winning odds with current combination **r<sub>1</sub>** and **t** rerolls already done.  

Let **A** be the set of all actions one player can take (for each dice wether or not it gets rerolled).  

![](https://latex.codecogs.com/svg.latex?g(r_1,r_2,0)=eval(r_1,r_2))  

![](https://latex.codecogs.com/svg.latex?g(r_1,r_2,t)=\min_{a}^{A}\mathbb{E}[g(f,X_{r_2a},t-1)])  

![](https://latex.codecogs.com/svg.latex?f(r_1,2)=\mathbb{E}[g(r_1,X_{},2)])  

![](https://latex.codecogs.com/svg.latex?f(r_1,t)=\max\begin{cases}\mathbb{E}[g(r_1,X_{},t)]\\\max_{a}^{A}\mathbb{E}[f(X_{r_1a},t&plus;1)]\end{cases})  

## Results

For an overall weighted winrate of `60.0044%` for the starting player.  

Table representation of **f**.  
Action column denotes the optimal strategy by showing which dice to keep.  
(For the second player, **g** results are available under [results folder](https://github.com/VincentPinet/421-Solver/tree/master/results/) with the facing roll as file name)

<table>
	<tr>
		<td rowspan="2"><b>r \ t </td>
		<td colspan="2"><b>0</td>
		<td colspan="2"><b>1</td>
		<td colspan="2"><b>2</td>
	</tr>
	<tr>
		<td>win%</td>
		<td>action</td>
		<td>win%</td>
		<td>action</td>
		<td>win%</td>
		<td>action</td>
	</tr>
	</tr>
		<td><b>421</td>
		<td>98.61%</td>
		<td>421</td>
		<td>94.23%</td>
		<td>421</td>
		<td>88.59%</td>
		<td>421</td>
	</tr>
	</tr>
		<td><b>111</td>
		<td>96.99%</td>
		<td>111</td>
		<td>87.94%</td>
		<td>111</td>
		<td>76.63%</td>
		<td>111</td>
	</tr>
	</tr>
		<td><b>611</td>
		<td>96.06%</td>
		<td>611</td>
		<td>85.80%</td>
		<td>611</td>
		<td>73.44%</td>
		<td>611</td>
	</tr>
	</tr>
		<td><b>666</td>
		<td>95.14%</td>
		<td>666</td>
		<td>83.60%</td>
		<td>666</td>
		<td>70.45%</td>
		<td>666</td>
	</tr>
	</tr>
		<td><b>511</td>
		<td>94.21%</td>
		<td>511</td>
		<td>82.02%</td>
		<td>511</td>
		<td>67.98%</td>
		<td>511</td>
	</tr>
	</tr>
		<td><b>555</td>
		<td>93.29%</td>
		<td>555</td>
		<td>79.42%</td>
		<td>555</td>
		<td>63.91%</td>
		<td>555</td>
	</tr>
	</tr>
		<td><b>411</td>
		<td>92.36%</td>
		<td>411</td>
		<td>76.90%</td>
		<td>411</td>
		<td>60.82%</td>
		<td>411</td>
	</tr>
	</tr>
		<td><b>444</td>
		<td>91.44%</td>
		<td>444</td>
		<td>73.99%</td>
		<td>444</td>
		<td>56.60%</td>
		<td>444</td>
	</tr>
	</tr>
		<td><b>311</td>
		<td>90.51%</td>
		<td>311</td>
		<td>72.30%</td>
		<td>311</td>
		<td>54.82%</td>
		<td>311</td>
	</tr>
	</tr>
		<td><b>333</td>
		<td>89.58%</td>
		<td>333</td>
		<td>70.84%</td>
		<td>333</td>
		<td>53.44%</td>
		<td>333</td>
	</tr>
	</tr>
		<td><b>211</td>
		<td>88.66%</td>
		<td>211</td>
		<td>69.38%</td>
		<td>211</td>
		<td>52.03%</td>
		<td>211</td>
	</tr>
	</tr>
		<td><b>222</td>
		<td>87.73%</td>
		<td>222</td>
		<td>67.98%</td>
		<td>222</td>
		<td>50.64%</td>
		<td>222</td>
	</tr>
	</tr>
		<td><b>654</td>
		<td>86.11%</td>
		<td>654</td>
		<td>66.24%</td>
		<td>654</td>
		<td>49.38%</td>
		<td>654</td>
	</tr>
	</tr>
		<td><b>543</td>
		<td>83.33%</td>
		<td>543</td>
		<td>62.54%</td>
		<td>543</td>
		<td>45.10%</td>
		<td>543</td>
	</tr>
	</tr>
		<td><b>432</td>
		<td>80.56%</td>
		<td>432</td>
		<td>58.29%</td>
		<td>432</td>
		<td>40.47%</td>
		<td>432</td>
	</tr>
	</tr>
		<td><b>321</td>
		<td>77.78%</td>
		<td>321</td>
		<td>53.51%</td>
		<td>321</td>
		<td>35.58%</td>
		<td>321</td>
	</tr>
	</tr>
		<td><b>665</td>
		<td>75.69%</td>
		<td>665</td>
		<td>49.40%</td>
		<td>665</td>
		<td>30.90%</td>
		<td>665</td>
	</tr>
	</tr>
		<td><b>664</td>
		<td>74.31%</td>
		<td>664</td>
		<td>47.22%</td>
		<td>664</td>
		<td>29.12%</td>
		<td>664</td>
	</tr>
	</tr>
		<td><b>663</td>
		<td>72.92%</td>
		<td>663</td>
		<td>45.66%</td>
		<td>663</td>
		<td>27.67%</td>
		<td>663</td>
	</tr>
	</tr>
		<td><b>662</td>
		<td>71.53%</td>
		<td>662</td>
		<td>44.44%</td>
		<td>662</td>
		<td>26.43%</td>
		<td>662</td>
	</tr>
	</tr>
		<td><b>661</td>
		<td>70.14%</td>
		<td>661</td>
		<td>42.88%</td>
		<td>661</td>
		<td>25.01%</td>
		<td>661</td>
	</tr>
	</tr>
		<td><b>655</td>
		<td>68.75%</td>
		<td>655</td>
		<td>41.38%</td>
		<td>655</td>
		<td>23.97%</td>
		<td>655</td>
	</tr>
	</tr>
		<td><b>653</td>
		<td>66.67%</td>
		<td>653</td>
		<td>38.73%</td>
		<td>653</td>
		<td>21.10%</td>
		<td>653</td>
	</tr>
	</tr>
		<td><b>652</td>
		<td>63.89%</td>
		<td>652</td>
		<td>35.03%</td>
		<td>652</td>
		<td>17.68%</td>
		<td>652</td>
	</tr>
	</tr>
		<td><b>651</td>
		<td>61.11%</td>
		<td>651</td>
		<td>31.75%</td>
		<td>651</td>
		<td>15.21%</td>
		<td>651</td>
	</tr>
	</tr>
		<td><b>644</td>
		<td>59.03%</td>
		<td>644</td>
		<td>29.75%</td>
		<td>644</td>
		<td>14.00%</td>
		<td>644</td>
	</tr>
	</tr>
		<td><b>643</td>
		<td>56.94%</td>
		<td>643</td>
		<td>28.01%</td>
		<td>643</td>
		<td>12.74%</td>
		<td>643</td>
	</tr>
	</tr>
		<td><b>642</td>
		<td>54.17%</td>
		<td>642</td>
		<td>25.04%</td>
		<td>642</td>
		<td>10.66%</td>
		<td>642</td>
	</tr>
	</tr>
		<td><b>641</td>
		<td>51.39%</td>
		<td>641</td>
		<td>29.15%</td>
		<td>1</td>
		<td> 8.79%</td>
		<td>641</td>
	</tr>
	</tr>
		<td><b>633</td>
		<td>49.31%</td>
		<td>633</td>
		<td>21.77%</td>
		<td>6</td>
		<td> 7.91%</td>
		<td>633</td>
	</tr>
	</tr>
		<td><b>632</td>
		<td>47.22%</td>
		<td>632</td>
		<td>21.77%</td>
		<td>6</td>
		<td> 7.43%</td>
		<td>632</td>
	</tr>
	</tr>
		<td><b>631</td>
		<td>49.81%</td>
		<td>1</td>
		<td>29.15%</td>
		<td>1</td>
		<td> 6.48%</td>
		<td>631</td>
	</tr>
	</tr>
		<td><b>622</td>
		<td>42.36%</td>
		<td>622</td>
		<td>21.77%</td>
		<td>6</td>
		<td> 5.85%</td>
		<td>622</td>
	</tr>
	</tr>
		<td><b>621</td>
		<td>51.35%</td>
		<td>21</td>
		<td>30.33%</td>
		<td>21</td>
		<td> 5.38%</td>
		<td>621</td>
	</tr>
	</tr>
		<td><b>554</td>
		<td>38.80%</td>
		<td>4</td>
		<td>19.69%</td>
		<td>4</td>
		<td> 4.97%</td>
		<td>554</td>
	</tr>
	</tr>
		<td><b>553</td>
		<td>38.54%</td>
		<td></td>
		<td>19.31%</td>
		<td></td>
		<td> 4.61%</td>
		<td>553</td>
	</tr>
	</tr>
		<td><b>552</td>
		<td>38.54%</td>
		<td></td>
		<td>19.31%</td>
		<td></td>
		<td> 4.09%</td>
		<td>552</td>
	</tr>
	</tr>
		<td><b>551</td>
		<td>49.81%</td>
		<td>1</td>
		<td>29.15%</td>
		<td>1</td>
		<td> 3.56%</td>
		<td>551</td>
	</tr>
	</tr>
		<td><b>544</td>
		<td>38.80%</td>
		<td>4</td>
		<td>19.69%</td>
		<td>4</td>
		<td> 3.33%</td>
		<td>544</td>
	</tr>
	</tr>
		<td><b>542</td>
		<td>41.50%</td>
		<td>42</td>
		<td>23.82%</td>
		<td>42</td>
		<td> 2.90%</td>
		<td>542</td>
	</tr>
	</tr>
		<td><b>541</td>
		<td>49.81%</td>
		<td>1</td>
		<td>29.15%</td>
		<td>1</td>
		<td> 1.80%</td>
		<td>541</td>
	</tr>
	</tr>
		<td><b>533</td>
		<td>38.54%</td>
		<td></td>
		<td>19.31%</td>
		<td></td>
		<td> 1.43%</td>
		<td>533</td>
	</tr>
	</tr>
		<td><b>532</td>
		<td>38.54%</td>
		<td></td>
		<td>19.31%</td>
		<td></td>
		<td> 1.11%</td>
		<td>532</td>
	</tr>
	</tr>
		<td><b>531</td>
		<td>49.81%</td>
		<td>1</td>
		<td>29.15%</td>
		<td>1</td>
		<td> 0.69%</td>
		<td>531</td>
	</tr>
	</tr>
		<td><b>522</td>
		<td>38.54%</td>
		<td></td>
		<td>19.31%</td>
		<td></td>
		<td> 0.49%</td>
		<td>522</td>
	</tr>
	</tr>
		<td><b>521</td>
		<td>51.35%</td>
		<td>21</td>
		<td>30.33%</td>
		<td>21</td>
		<td> 0.39%</td>
		<td>521</td>
	</tr>
	</tr>
		<td><b>443</td>
		<td>38.80%</td>
		<td>4</td>
		<td>19.69%</td>
		<td>4</td>
		<td> 0.32%</td>
		<td>443</td>
	</tr>
	</tr>
		<td><b>442</td>
		<td>41.50%</td>
		<td>42</td>
		<td>23.82%</td>
		<td>42</td>
		<td> 0.25%</td>
		<td>442</td>
	</tr>
	</tr>
		<td><b>441</td>
		<td>49.81%</td>
		<td>1</td>
		<td>29.15%</td>
		<td>1</td>
		<td> 0.18%</td>
		<td>441</td>
	</tr>
	</tr>
		<td><b>433</td>
		<td>38.80%</td>
		<td>4</td>
		<td>19.69%</td>
		<td>4</td>
		<td> 0.12%</td>
		<td>433</td>
	</tr>
	</tr>
		<td><b>431</td>
		<td>49.81%</td>
		<td>1</td>
		<td>29.15%</td>
		<td>1</td>
		<td> 0.06%</td>
		<td>431</td>
	</tr>
	</tr>
		<td><b>422</td>
		<td>41.50%</td>
		<td>42</td>
		<td>23.82%</td>
		<td>42</td>
		<td> 0.02%</td>
		<td>422</td>
	</tr>
	</tr>
		<td><b>332</td>
		<td>38.54%</td>
		<td></td>
		<td>19.31%</td>
		<td></td>
		<td> 0.02%</td>
		<td>332</td>
	</tr>
	</tr>
		<td><b>331</td>
		<td>49.81%</td>
		<td>1</td>
		<td>29.15%</td>
		<td>1</td>
		<td> 0.01%</td>
		<td>331</td>
	</tr>
	</tr>
		<td><b>322</td>
		<td>38.54%</td>
		<td></td>
		<td>19.31%</td>
		<td></td>
		<td> 0.00%</td>
		<td>322</td>
	</tr>
	</tr>
		<td><b>221</td>
		<td>51.35%</td>
		<td>21</td>
		<td>30.33%</td>
		<td>21</td>
		<td> 0.00%</td>
		<td>221</td>
	</tr>
</table>
