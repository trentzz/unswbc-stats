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

### Example Output
```
Session Date: 20230415
Games played this session: 63

Teams that have played more than once:
calvin, soren, wenyu, yif: 5
jaden, jian, leo, trent: 4
ben, jophy, jwu, xinxin: 2
ian, jus, paco, q: 2
jackson, jazz, oscar, vincent: 2
jess, shane, yaning, yasmin: 2
karen, shitong, yibo, zz: 2
karen, xinda, yibo, zz: 2

Court Usage Stats:
1: 11
4: 11
5: 11
6: 11
3: 10
2: 9

Games played:
vincent: 7
shitong: 6
tk: 5
xavier: 5
kenward: 5
yibo: 5
carina: 5
leo: 5
yifan: 5
soren: 5
paco: 5
maria: 5
kiki: 5
yan: 5
jaden: 5
jian: 5
xinda: 5
cyrus: 5
jess: 5
qamal: 5
yif: 5
q: 5
stevo: 5
trent: 5
calvin: 5
wenyu: 5
xinxin: 4
oscar: 4
nikita: 4
sandy: 4
yasmin: 4
jophy: 4
zz: 4
jackson: 4
ben: 4
chris: 4
karen: 4
yuka: 4
hantao: 4
luke: 4
ian: 3
emma: 3
frank: 3
risa: 3
jus: 3
joseph: 3
naoko: 3
jazz: 3
phillis: 3
raymond: 3
dheeraj: 3
yaning: 3
nishit: 3
john wu: 3
chui: 3
chewy: 2
deeraj: 2
liting: 2
jwu: 2
shane: 2
philis: 2
moki: 2
bora: 2
nikitas: 1
jonathan: 1
```
