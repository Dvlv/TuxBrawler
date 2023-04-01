## Tux Brawler

FOSS Arena fighter game / engine

## TODO

### Brawlers
- [ ] Parse brawler json files into a vec of unique / shared ptrs
  - [ ] Pass these to the char select state
  - [ ] Pass / move only the selected fighters into the brawl state
- [ ] Implement animation system
- [ ] Implement state machine
  - [ ] Moving
  - [ ] Attacking
  - [ ] Jumping / In Air
  - [ ] Hitstun
  - [ ] Blocking

### Maps
- [ ] Maps class
    - [ ] Map JSON structure
    - [ ] Parse map structure into class
- [ ] Pass parsed maps to map select state
- [ ] Pass only selected map to brawl state


### States
- [ ] Map Select
- [ ] Char Select
- [ ] Brawl
  - [ ] 3 2 1 GO
  - [ ] Pass map collisions to player update
  - [ ] Off edge of screen = die
  - [ ] Score system
  - [ ] Timer


### Long way off
- [ ] Projectiles
    - [ ] Items?
