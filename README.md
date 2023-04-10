## Tux Brawler

FOSS Arena fighter game / engine

## TODO

### Brawlers
- [x] Parse brawler json files into a vec of unique / shared ptrs
  - [x] Pass these to the char select state
  - [x] Pass / move only the selected fighters into the brawl state

- [x] Implement animation system

- [ ] Attack System
  - [ ] Animation
  - [ ] Atk Hitbox
  - [ ] Check collision with other brawlers in hitbox
  - [ ] hitstun
    - [ ] bounce off walls
    - [ ] DI?

- [ ] Allow hitbox to be passed in JSON


#### AI
- [ ] Implement state machine
  - [ ] Moving
  - [ ] Attacking
  - [ ] Jumping / In Air
  - [ ] Hitstun
  - [ ] Blocking

### Maps
- [x] Maps class
    - [x] Map JSON structure
    - [x] Parse map structure into class
- [ ] Pass parsed maps to map select state
- [ ] Pass only selected map to brawl state


### States
- [ ] Map Select
- [x] Char Select
- [ ] Brawl
  - [ ] 3 2 1 GO
  - [ ] Pass map collisions to player update
  - [ ] Off edge of screen = die
  - [ ] Score system
  - [ ] Timer


### Long way off
- [ ] Projectiles
    - [ ] Items?
