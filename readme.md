## UNSW BC Stats

no flame pls i wrote this at 2am

full disclosure code is spaghetti

### Supported Features

- Number of games each player has played.
- Number of games each team has played together in a session.
- Number of times each court has been used per session.

### Future Features

- Number of sessions each player has attended.
- Number of games a pair of players have played together.
- Number of times a court has been used overall.
- Number of games a team has played together overall.

### Usage
1. At the end of each session, copy the court and player data to a new google sheet. They should have the columns: `COURT | PLAYER 1 | PLAYER 2 | PLAYER 3 | PLAYER 4 |`
2. Download all google sheets for games.
3. Run `bash xlsx_to_csv.sh` to convert .xlsx files to .csv. Make sure the
   .xlsx files are in a folder called "google\_sheets". Feel free to change
   this, and update it in the bash file.
4. Compile and run `unswbc_stats.cpp` to get stats for all files.
