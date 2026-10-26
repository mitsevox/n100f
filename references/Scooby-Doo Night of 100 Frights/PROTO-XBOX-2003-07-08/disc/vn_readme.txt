Proposed Villain HIP files.

Purpose:
--------
To reduce the amount of raw data which must be read from disk/CDROM/DVD/etc.
when switching between "rooms" in the game engine.

Overview:
---------
The Scooby-Doo game consists of a 4 "Worlds" (Smugglers Cove, Haunted
Grounds, etc.) which is composed of 3 "Levels" (denoted by the 'f in
f001.HIP), and each level consists of a series of "Rooms" (f001, f002, ...).

The rooms within each level tend to use the same villains to defend the
MasterMind from our hero Scooby. Further, the progression of the game
introduces a subset of villains as the player achieves greater glory and
is victorious on the battle field (or at least quick-footed enough to
avoid confrontation).

This frequency of loading the same data can be reduced by splitting
villains into their own HIP files which are loaded based on the level
of the room being entered.
(similar to how the Boot HIP loads the player data once)

Benefits:
---------

+ Currently, each room (aka scene) loads the animation and model data
  required for those villains in the room. In most levels, this results in
  redundantly loading 1.5 to 2 MB of data. Two MB is, presently,
  approximately 25% to 40% of the total amount of data loaded for each
  room - another third is in the textures, a third is the BSP, plus
  other smaller stuff makes up the rest.

  Thus, switching rooms within a given level can skip loading a fair
  amount of data. This reduces the load times when moving between rooms.

+ Limits/forces designers to only use villains appropriate to the level.
  Requires, of course, the villains which are to be used in each level
  are known so they can be packed accordingly.

+ Specifically, it will help standardize big-ticket memory 
  usage across scenes. Thus quality of scenes won't be hampered 
  because one scene has tons of different villains while 
  another doesn't ... sorta like a mid-tide mark for scene content.
  Others are welcome to disagree.

+ Critters (bats, rats, spiders, etc.) are rather ubiquitous in the game.
  Consequently they are placed in their own villain HIP which is loaded
  before the other villain HIPs - actually, they could go in the boot but
  using a separate HIP keeps things more straight forward.

+ Since these are broken down by level and room, provides a pack location
  for sounds (like background music) and other assets which pertain to the
  level as a whole.

Drawbacks:
----------
Among others, ....

- Slightly more complex packing/loading sequence. However, it has been done
  for the UI menus and the boot HIP.

- Moving between worlds and levels will take the hit of loading a different
  villain HIP. Hopefully, the design and layout of room connectivity
  limits how often this can occur.

- Villains used in a scene which are not present in the 
  villain HIP file will cause problems (i.e. crash); a normal response
  for missing game assets.

- Villains used throughout a level will always be loaded for 
  rooms within those levels whether used or not.

  This is wasteful for villains only seen once - like a boss.
  As a result, bosses, and the villains used within the Boss Battle room
  may be placed in one of the 9'th HIP for that level (see "HIP Names" below).
  Splitting bosses off to another HIP may be avoided if unnecessary.
 
HIP Names:
----------
Each HIP is named as follows:

VNab
  'a'  indicates world/level supported by the HIP. This is the alpha
       character used to designate rooms within a level (e.g., 'F' in 'F001')
  'b'  enumeration digit. Typically, there will only be a single
       'VN**' HIP for all rooms within the level. E.g., "VNF1".
       The exception will probably be limited to the world levels
       containing a boss battle.

       The villain HIP used for a boss battle will always have b == 9.

       For now at least, b == 0 is reserved for testing/development purposes.

       The boss versions may be omitted if unnecessary. However, that would
       limit other rooms on the level since boss animations would always
       be loaded when not required until the end. And, those rooms
       in the level should really be more complex/challenging since they
       lead up to the boss battle.

Anyway:
    Room     'g009'     VNg9          - Green Ghost Battle
    Room     's005'     VNs9          - Master Mind Battle
    Room     'o008'     VNo9          - Black Knight Battle
    Room     'w028'     VNw9          - Red Beard Battle
otherwise, levels are VNf1, VNp1, etc.


Load Ordering:
--------------

    ---------------
    |    Room     |      - Room HIP (single room)
    ---------------
    |    VN--     |      - level specific villains (per level)
    ---------------
    |    VN00     |      - common villains (e.g. critters) (whole game)
    ---------------
    |    MNU4     |      - HUD and option screens (whole game)
    ---------------
    |    BOOT     |      - Player and game generic data (whole game)
    ---------------

