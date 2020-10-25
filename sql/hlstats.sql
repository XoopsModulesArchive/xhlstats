#
# Table structure for table 'hlstats_Actions'
#

CREATE TABLE hlstats_Actions (
    id                      INT(10) UNSIGNED                NOT NULL AUTO_INCREMENT,
    game                    VARCHAR(16)     DEFAULT 'valve' NOT NULL,
    code                    VARCHAR(64)     DEFAULT ''      NOT NULL,
    reward_player           INT(11)         DEFAULT '10'    NOT NULL,
    reward_team             INT(11)         DEFAULT '0'     NOT NULL,
    team                    VARCHAR(32)     DEFAULT ''      NOT NULL,
    description             VARCHAR(128),
    for_PlayerActions       ENUM ('0', '1') DEFAULT '0'     NOT NULL,
    for_PlayerPlayerActions ENUM ('0', '1') DEFAULT '0'     NOT NULL,
    for_TeamActions         ENUM ('0', '1') DEFAULT '0'     NOT NULL,
    for_WorldActions        ENUM ('0', '1') DEFAULT '0'     NOT NULL,
    PRIMARY KEY (id),
    KEY code (code),
    UNIQUE gamecode (game, code)
);

#
# Table structure for table 'hlstats_Awards'
#

CREATE TABLE hlstats_Awards (
    awardId        INT(10) UNSIGNED               NOT NULL AUTO_INCREMENT,
    awardType      ENUM ('W','O') DEFAULT 'W'     NOT NULL,
    game           VARCHAR(32)    DEFAULT 'valve' NOT NULL,
    code           VARCHAR(128)   DEFAULT ''      NOT NULL,
    name           VARCHAR(128)   DEFAULT ''      NOT NULL,
    verb           VARCHAR(64)    DEFAULT ''      NOT NULL,
    d_winner_id    INT(10) UNSIGNED,
    d_winner_count INT(10) UNSIGNED,
    PRIMARY KEY (awardId),
    UNIQUE KEY code (game, awardType, code)
);

#
# Table structure for table 'hlstats_Clans'
#

CREATE TABLE hlstats_Clans (
    clanId   INT(10) UNSIGNED        NOT NULL AUTO_INCREMENT,
    tag      VARCHAR(32)  DEFAULT '' NOT NULL,
    name     VARCHAR(128) DEFAULT '' NOT NULL,
    homepage VARCHAR(64)  DEFAULT '' NOT NULL,
    game     VARCHAR(16)  DEFAULT '' NOT NULL,
    PRIMARY KEY (clanId),
    KEY game (game),
    UNIQUE tag (game, tag)
);

#
# Table structure for table 'hlstats_ClanTags'
#

CREATE TABLE hlstats_ClanTags (
    id       INT(10) UNSIGNED                               NOT NULL AUTO_INCREMENT,
    pattern  VARCHAR(64)                   DEFAULT ''       NOT NULL,
    position ENUM ('EITHER','START','END') DEFAULT 'EITHER' NOT NULL,
    PRIMARY KEY (id),
    UNIQUE KEY pattern (pattern)
);

#
# Dumping data for table 'hlstats_ClanTags'
#

INSERT INTO hlstats_ClanTags
VALUES (1, '[AXXXXX]', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (2, '|AXXXXX|', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (3, '(AXXXXX)', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (4, '{AXXXXX}', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (5, '-=AXXX=-', 'START');
INSERT INTO hlstats_ClanTags
VALUES (6, '=AXXXXX=', 'START');
INSERT INTO hlstats_ClanTags
VALUES (7, '-AXXXXX-', 'START');
INSERT INTO hlstats_ClanTags
VALUES (8, '=[AXXXX]=', 'START');
INSERT INTO hlstats_ClanTags
VALUES (9, '-=|AXXXXXX|=-', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (10, '-=[AXXXXXX]=-', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (11, '-=]AXXXXXX[=-', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (12, '~{|AXXXXXX|}~', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (13, '-|-AXXXXXX-|-', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (14, '-(AXXXXXX)-', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (15, '::AXXXXXX::', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (16, '<<AXXXXXX>>', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (17, '{{AXXXXXX}}', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (18, '((AXXXXXX))', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (19, '.|AXXXXXX|.', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (20, '--AXXXXXX--', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (21, '-)AXXXXXX(-', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (22, '/AXXXXXX\\', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (23, '//AXXXXXX\\\\', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (24, '_AXXXXXX_', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (25, '_=|AXXXXXX|=_', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (26, '*AXXXXXX*', 'EITHER');
INSERT INTO hlstats_ClanTags
VALUES (27, '.:AXXXXXX:', 'START');
INSERT INTO hlstats_ClanTags
VALUES (28, '[(AXXXXXX)]', 'EITHER');

#
# Table structure for table 'hlstats_Games'
#

CREATE TABLE hlstats_Games (
    code   VARCHAR(16)     DEFAULT ''  NOT NULL,
    name   VARCHAR(128)    DEFAULT ''  NOT NULL,
    hidden ENUM ('0', '1') DEFAULT '0' NOT NULL,
    PRIMARY KEY (code)
);

#
# Table structure for table 'hlstats_HostGroups'
#

CREATE TABLE hlstats_HostGroups (
    id      INT(11)                 NOT NULL AUTO_INCREMENT,
    pattern VARCHAR(128) DEFAULT '' NOT NULL,
    name    VARCHAR(128) DEFAULT '' NOT NULL,
    PRIMARY KEY (id)
);

#
# Dumping data for table 'hlstats_HostGroups'
#

INSERT INTO hlstats_HostGroups
VALUES (NULL, '.aol.com', 'AOL');

#
# Table structure for table 'hlstats_Options'
#

CREATE TABLE hlstats_Options (
    keyname VARCHAR(32)  DEFAULT '' NOT NULL,
    value   VARCHAR(255) DEFAULT '' NOT NULL,
    PRIMARY KEY (keyname)
);

#
# Dumping data for table 'hlstats_Options'
#

INSERT INTO hlstats_Options
VALUES ('imgdir', 'images');
INSERT INTO hlstats_Options
VALUES ('contact', 'mailto:admin@example.com');
INSERT INTO hlstats_Options
VALUES ('sitename', 'Some Site');
INSERT INTO hlstats_Options
VALUES ('siteurl', 'http://www.example.com');

#
# Table structure for table 'hlstats_Players'
#

CREATE TABLE hlstats_Players (
    playerId    INT(10) UNSIGNED                NOT NULL AUTO_INCREMENT,
    lastName    VARCHAR(64)      DEFAULT ''     NOT NULL,
    clan        INT(10) UNSIGNED DEFAULT '0'    NOT NULL,
    kills       INT(11)          DEFAULT '0'    NOT NULL,
    deaths      INT(11)          DEFAULT '0'    NOT NULL,
    suicides    INT(11)          DEFAULT '0'    NOT NULL,
    skill       INT(11)          DEFAULT '1000' NOT NULL,
    fullName    VARCHAR(128),
    email       VARCHAR(128),
    homepage    VARCHAR(128),
    icq         INT(10) UNSIGNED,
    game        VARCHAR(16)      DEFAULT ''     NOT NULL,
    hideranking INT(1) UNSIGNED  DEFAULT '0'    NOT NULL,
    PRIMARY KEY (playerId),
    KEY clan (clan),
    KEY game (game)
);

#
# Table structure for table 'hlstats_PlayerNames'
#

CREATE TABLE hlstats_PlayerNames (
    playerId INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    name     VARCHAR(64)      DEFAULT ''                    NOT NULL,
    lastuse  DATETIME         DEFAULT '0000-00-00 00:00:00' NOT NULL,
    numuses  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    kills    INT(11)          DEFAULT '0'                   NOT NULL,
    deaths   INT(11)          DEFAULT '0'                   NOT NULL,
    suicides INT(11)          DEFAULT '0'                   NOT NULL,
    PRIMARY KEY (playerId, name)
);

#
# Table structure for table 'hlstats_PlayerUniqueIds'
#

CREATE TABLE hlstats_PlayerUniqueIds (
    playerId INT(10) UNSIGNED DEFAULT '0' NOT NULL,
    uniqueId VARCHAR(64)      DEFAULT ''  NOT NULL,
    game     VARCHAR(16)      DEFAULT ''  NOT NULL,
    merge    INT(10) UNSIGNED,
    KEY playerId (playerId),
    PRIMARY KEY (uniqueId, game)
);

#
# Table structure for table 'hlstats_Roles'
#

CREATE TABLE hlstats_Roles (
    roleId INT(10) UNSIGNED                NOT NULL AUTO_INCREMENT,
    game   VARCHAR(16)     DEFAULT 'valve' NOT NULL,
    code   VARCHAR(32)     DEFAULT ''      NOT NULL,
    name   VARCHAR(64)     DEFAULT ''      NOT NULL,
    hidden ENUM ('0', '1') DEFAULT '0'     NOT NULL,
    PRIMARY KEY (roleId),
    UNIQUE gamecode (game, code)
);

#
# Table structure for table 'hlstats_Servers'
#

CREATE TABLE hlstats_Servers (
    serverId      INT(10) UNSIGNED                NOT NULL AUTO_INCREMENT,
    address       VARCHAR(15)     DEFAULT ''      NOT NULL,
    port          INT(5) UNSIGNED DEFAULT '0'     NOT NULL,
    name          VARCHAR(64)     DEFAULT ''      NOT NULL,
    game          VARCHAR(16)     DEFAULT 'valve' NOT NULL,
    publicaddress VARCHAR(64)     DEFAULT ''      NOT NULL,
    statusurl     VARCHAR(255),
    rcon_password VARCHAR(48)     DEFAULT ''      NOT NULL,
    PRIMARY KEY (serverId),
    UNIQUE addressport (address, port)
);

#
# Table structure for table 'hlstats_Teams'
#

CREATE TABLE hlstats_Teams (
    teamId INT(10) UNSIGNED                NOT NULL AUTO_INCREMENT,
    game   VARCHAR(16)     DEFAULT 'valve' NOT NULL,
    code   VARCHAR(32)     DEFAULT ''      NOT NULL,
    name   VARCHAR(64)     DEFAULT ''      NOT NULL,
    hidden ENUM ('0', '1') DEFAULT '0'     NOT NULL,
    PRIMARY KEY (teamId),
    UNIQUE gamecode (game, code)
);


#
# Table structure for table 'hlstats_Weapons'
#

CREATE TABLE hlstats_Weapons (
    weaponId INT(10) UNSIGNED             NOT NULL AUTO_INCREMENT,
    game     VARCHAR(16)  DEFAULT 'valve' NOT NULL,
    code     VARCHAR(32)  DEFAULT ''      NOT NULL,
    name     VARCHAR(64)  DEFAULT ''      NOT NULL,
    modifier FLOAT(10, 2) DEFAULT '1.00'  NOT NULL,
    PRIMARY KEY (weaponId),
    UNIQUE gamecode (game, code)
);


##
## Events Tables
##

#
# Table structure for table 'hlstats_Events_Admin'
#

CREATE TABLE hlstats_Events_Admin (
    id         INT(10) UNSIGNED                               NOT NULL AUTO_INCREMENT,
    eventTime  DATETIME         DEFAULT '0000-00-00 00:00:00' NOT NULL,
    serverId   INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    map        VARCHAR(32)      DEFAULT ''                    NOT NULL,
    type       VARCHAR(32)      DEFAULT 'Unknown'             NOT NULL,
    message    VARCHAR(128)     DEFAULT ''                    NOT NULL,
    playerName VARCHAR(128)     DEFAULT ''                    NOT NULL,
    PRIMARY KEY (id)
);

#
# Table structure for table 'hlstats_Events_ChangeName'
#

CREATE TABLE hlstats_Events_ChangeName (
    id        INT(10) UNSIGNED                               NOT NULL AUTO_INCREMENT,
    eventTime DATETIME         DEFAULT '0000-00-00 00:00:00' NOT NULL,
    serverId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    map       VARCHAR(20)      DEFAULT ''                    NOT NULL,
    playerId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    oldName   VARCHAR(64)      DEFAULT ''                    NOT NULL,
    newName   VARCHAR(64)      DEFAULT ''                    NOT NULL,
    PRIMARY KEY (id)
);

#
# Table structure for table 'hlstats_Events_ChangeRole'
#

CREATE TABLE hlstats_Events_ChangeRole (
    id        INT(10) UNSIGNED                               NOT NULL AUTO_INCREMENT,
    eventTime DATETIME         DEFAULT '0000-00-00 00:00:00' NOT NULL,
    serverId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    map       VARCHAR(20)      DEFAULT ''                    NOT NULL,
    playerId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    role      VARCHAR(32)      DEFAULT ''                    NOT NULL,
    PRIMARY KEY (id)
);

#
# Table structure for table 'hlstats_Events_ChangeTeam'
#

CREATE TABLE hlstats_Events_ChangeTeam (
    id        INT(10) UNSIGNED                               NOT NULL AUTO_INCREMENT,
    eventTime DATETIME         DEFAULT '0000-00-00 00:00:00' NOT NULL,
    serverId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    map       VARCHAR(20)      DEFAULT ''                    NOT NULL,
    playerId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    team      VARCHAR(32)      DEFAULT ''                    NOT NULL,
    PRIMARY KEY (id)
);

#
# Table structure for table 'hlstats_Events_Connects'
#

CREATE TABLE hlstats_Events_Connects (
    id        INT(10) UNSIGNED                               NOT NULL AUTO_INCREMENT,
    eventTime DATETIME         DEFAULT '0000-00-00 00:00:00' NOT NULL,
    serverId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    map       VARCHAR(32)      DEFAULT ''                    NOT NULL,
    playerId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    ipAddress VARCHAR(15)      DEFAULT ''                    NOT NULL,
    hostname  VARCHAR(128)     DEFAULT ''                    NOT NULL,
    hostgroup VARCHAR(128)     DEFAULT ''                    NOT NULL,
    PRIMARY KEY (id)
);

#
# Table structure for table 'hlstats_Events_Disconnects'
#

CREATE TABLE hlstats_Events_Disconnects (
    id        INT(10) UNSIGNED                               NOT NULL AUTO_INCREMENT,
    eventTime DATETIME         DEFAULT '0000-00-00 00:00:00' NOT NULL,
    serverId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    map       VARCHAR(20)      DEFAULT ''                    NOT NULL,
    playerId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    PRIMARY KEY (id)
);

#
# Table structure for table 'hlstats_Events_Entries'
#

CREATE TABLE hlstats_Events_Entries (
    id        INT(10) UNSIGNED                               NOT NULL AUTO_INCREMENT,
    eventTime DATETIME         DEFAULT '0000-00-00 00:00:00' NOT NULL,
    serverId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    map       VARCHAR(20)      DEFAULT ''                    NOT NULL,
    playerId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    PRIMARY KEY (id)
);

#
# Table structure for table 'hlstats_Events_Frags'
#

CREATE TABLE hlstats_Events_Frags (
    id        INT(10) UNSIGNED                               NOT NULL AUTO_INCREMENT,
    eventTime DATETIME         DEFAULT '0000-00-00 00:00:00' NOT NULL,
    serverId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    map       VARCHAR(20)      DEFAULT ''                    NOT NULL,
    killerId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    victimId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    weapon    VARCHAR(20)      DEFAULT ''                    NOT NULL,
    PRIMARY KEY (id)
);

#
# Table structure for table 'hlstats_Events_PlayerActions'
#

CREATE TABLE hlstats_Events_PlayerActions (
    id        INT(10) UNSIGNED                               NOT NULL AUTO_INCREMENT,
    eventTime DATETIME         DEFAULT '0000-00-00 00:00:00' NOT NULL,
    serverId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    map       VARCHAR(20)      DEFAULT ''                    NOT NULL,
    playerId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    actionId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    bonus     INT(11)          DEFAULT '0'                   NOT NULL,
    PRIMARY KEY (id)
);

#
# Table structure for table 'hlstats_Events_PlayerPlayerActions'
#

CREATE TABLE hlstats_Events_PlayerPlayerActions (
    id        INT(10) UNSIGNED                               NOT NULL AUTO_INCREMENT,
    eventTime DATETIME         DEFAULT '0000-00-00 00:00:00' NOT NULL,
    serverId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    map       VARCHAR(20)      DEFAULT ''                    NOT NULL,
    playerId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    victimId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    actionId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    bonus     INT(11)          DEFAULT '0'                   NOT NULL,
    PRIMARY KEY (id)
);

#
# Table structure for table 'hlstats_Events_Rcon'
#

CREATE TABLE hlstats_Events_Rcon (
    id        INT(10) UNSIGNED                               NOT NULL AUTO_INCREMENT,
    eventTime DATETIME         DEFAULT '0000-00-00 00:00:00' NOT NULL,
    serverId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    map       VARCHAR(32)      DEFAULT ''                    NOT NULL,
    type      VARCHAR(6)       DEFAULT 'UNK'                 NOT NULL,
    remoteIp  VARCHAR(15)      DEFAULT ''                    NOT NULL,
    password  VARCHAR(32)      DEFAULT ''                    NOT NULL,
    command   VARCHAR(128)     DEFAULT ''                    NOT NULL,
    PRIMARY KEY (id)
);

#
# Table structure for table 'hlstats_Events_Suicides'
#

CREATE TABLE hlstats_Events_Suicides (
    id        INT(10) UNSIGNED                               NOT NULL AUTO_INCREMENT,
    eventTime DATETIME         DEFAULT '0000-00-00 00:00:00' NOT NULL,
    serverId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    map       VARCHAR(20)      DEFAULT ''                    NOT NULL,
    playerId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    weapon    VARCHAR(20)      DEFAULT ''                    NOT NULL,
    PRIMARY KEY (id)
);

#
# Table structure for table 'hlstats_Events_TeamBonuses'
#

CREATE TABLE hlstats_Events_TeamBonuses (
    id        INT(10) UNSIGNED                               NOT NULL AUTO_INCREMENT,
    eventTime DATETIME         DEFAULT '0000-00-00 00:00:00' NOT NULL,
    serverId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    map       VARCHAR(32)      DEFAULT ''                    NOT NULL,
    playerId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    actionId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    bonus     INT(11)          DEFAULT '0'                   NOT NULL,
    PRIMARY KEY (id)
);

#
# Table structure for table 'hlstats_Events_Teamkills'
#

CREATE TABLE hlstats_Events_Teamkills (
    id        INT(10) UNSIGNED                               NOT NULL AUTO_INCREMENT,
    eventTime DATETIME         DEFAULT '0000-00-00 00:00:00' NOT NULL,
    serverId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    map       VARCHAR(20)      DEFAULT ''                    NOT NULL,
    killerId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    victimId  INT(10) UNSIGNED DEFAULT '0'                   NOT NULL,
    weapon    VARCHAR(20)      DEFAULT ''                    NOT NULL,
    PRIMARY KEY (id)
);

#
# Table structure for table 'hlstats_Events_Statsme'
#

CREATE TABLE hlstats_Events_Statsme (
    id        INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
    eventTime DATETIME         NOT NULL DEFAULT '0000-00-00 00:00:00',
    serverId  INT(10) UNSIGNED NOT NULL DEFAULT '0',
    map       VARCHAR(32)      NOT NULL DEFAULT '',
    playerId  INT(10) UNSIGNED NOT NULL DEFAULT '0',
    weapon    VARCHAR(32)      NOT NULL DEFAULT '',
    shots     INT(6) UNSIGNED  NOT NULL DEFAULT '0',
    hits      INT(6) UNSIGNED  NOT NULL DEFAULT '0',
    headshots INT(6) UNSIGNED  NOT NULL DEFAULT '0',
    damage    INT(6) UNSIGNED  NOT NULL DEFAULT '0',
    kills     INT(6) UNSIGNED  NOT NULL DEFAULT '0',
    deaths    INT(6) UNSIGNED  NOT NULL DEFAULT '0',
    KEY playerId (playerId),
    PRIMARY KEY (id)
)
    ENGINE = ISAM;

#
# Table structure for table 'hlstats_Events_Statsme2'
#

CREATE TABLE hlstats_Events_Statsme2 (
    id        INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
    eventTime DATETIME         NOT NULL DEFAULT '0000-00-00 00:00:00',
    serverId  INT(10) UNSIGNED NOT NULL DEFAULT '0',
    map       VARCHAR(32)      NOT NULL DEFAULT '',
    playerId  INT(10) UNSIGNED NOT NULL DEFAULT '0',
    weapon    VARCHAR(32)      NOT NULL DEFAULT '',
    head      INT(6) UNSIGNED  NOT NULL DEFAULT '0',
    chest     INT(6) UNSIGNED  NOT NULL DEFAULT '0',
    stomach   INT(6) UNSIGNED  NOT NULL DEFAULT '0',
    leftarm   INT(6) UNSIGNED  NOT NULL DEFAULT '0',
    rightarm  INT(6) UNSIGNED  NOT NULL DEFAULT '0',
    leftleg   INT(6) UNSIGNED  NOT NULL DEFAULT '0',
    rightleg  INT(6) UNSIGNED  NOT NULL DEFAULT '0',
    KEY playerId (playerId),
    PRIMARY KEY (id)
)
    ENGINE = ISAM;

#
# Table structure for table 'hlstats_Events_StatsmeTime'
#

CREATE TABLE hlstats_Events_StatsmeTime (
    id        INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
    eventTime DATETIME         NOT NULL DEFAULT '0000-00-00 00:00:00',
    serverId  INT(10) UNSIGNED NOT NULL DEFAULT '0',
    map       VARCHAR(32)      NOT NULL DEFAULT '',
    playerId  INT(10) UNSIGNED NOT NULL DEFAULT '0',
    time      TIME             NOT NULL DEFAULT '00:00:00',
    KEY playerId (playerId),
    PRIMARY KEY (id)
)
    ENGINE = ISAM;

#
# Table structure for table 'hlstats_Events_StatsmeLatency'
#

CREATE TABLE hlstats_Events_StatsmeLatency (
    id        INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
    eventTime DATETIME         NOT NULL DEFAULT '0000-00-00 00:00:00',
    serverId  INT(10) UNSIGNED NOT NULL DEFAULT '0',
    map       VARCHAR(32)      NOT NULL DEFAULT '',
    playerId  INT(10) UNSIGNED NOT NULL DEFAULT '0',
    ping      INT(6) UNSIGNED  NOT NULL DEFAULT '0',
    KEY playerId (playerId),
    PRIMARY KEY (id)
)
    ENGINE = ISAM;

#
# Table structure for table `hlstats_server_addons`
#

CREATE TABLE hlstats_server_addons (
    rule  VARCHAR(64)  NOT NULL DEFAULT '',
    addon VARCHAR(64)  NOT NULL DEFAULT '',
    url   VARCHAR(255) NOT NULL DEFAULT '',
    PRIMARY KEY (rule)
)
    ENGINE = ISAM;

#
# Dumping data for table `hlstats_server_addons`
#

INSERT INTO hlstats_server_addons (`rule`, `addon`, `url`)
VALUES ('hlg_version', 'HLGuard %', 'http://www.unitedadmins.com/hlguard.php');
INSERT INTO hlstats_server_addons (`rule`, `addon`, `url`)
VALUES ('clanmod_version', 'ClanMod %', 'http://www.unitedadmins.com/clanmod.php');
INSERT INTO hlstats_server_addons (`rule`, `addon`, `url`)
VALUES ('statsme_version', 'StatsMe %', 'http://www.unitedadmins.com/statsme.php');
INSERT INTO hlstats_server_addons (`rule`, `addon`, `url`)
VALUES ('phpua_mm_version', 'phpUA %', 'http://www.phpua.com');
INSERT INTO hlstats_server_addons (`rule`, `addon`, `url`)
VALUES ('cdversion', 'Cheating-Death %', 'http://www.unitedadmins.com/cdeath.php');
INSERT INTO hlstats_server_addons (`rule`, `addon`, `url`)
VALUES ('metamod_version', 'MetaMod %', 'http://www.metamod.org');
INSERT INTO hlstats_server_addons (`rule`, `addon`, `url`)
VALUES ('amx_version', 'AMX %', 'http://www.amxmod.net');
INSERT INTO hlstats_server_addons (`rule`, `addon`, `url`)
VALUES ('amxmodx_version', 'AMXMODX %', 'http://www.amxmodx.org');


#
# GameSupport inserts
#

# Natural Selection

#
# Game Definition
#
INSERT INTO hlstats_Games
VALUES ('ns', 'Natural Selection', '0');


#
# Awards
#
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'ns', 'slash', 'Vicious Kitty', 'killings by le Swipe', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'ns', 'shotgun', 'Buckshot Masta', 'killings with the shotty', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'ns', 'pistol', 'Harold Handgun Alert', 'asskickings by pistola', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'ns', 'knife', 'Iron Chef Alert', 'vicious stabbings', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'ns', 'grenade', 'absolute n00b', 'pathetic killings by n00b grenades', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'ns', 'bitegun', 'Teething Tommy', 'killings with le jaw', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'ns', 'bite2gun', 'Mouth Full', 'killings with le big jaw', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'ns', 'leap', 'Tigger Alert', 'crushings by leap', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'ns', 'divinewind', 'Silent but Violent', 'slayings by recal relief', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'ns', 'sporegunprojectile', 'Left Feet Larry', 'killings with Lerk', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'ns', 'devour', 'Hungry Hungry Hippo', 'killings by Ingestion', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'ns', 'spitgunspit', 'Masta Fatty', 'Marines too dumb to kill a gorge', NULL, NULL);


#
# Actions
#
INSERT INTO hlstats_Actions
VALUES (NULL, 'ns', 'structure_built', 1, 0, '', 'Structures Built', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'ns', 'structure_destroyed', 2, 0, '', 'Structures Destroyed', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'ns', 'research_start', 1, 0, '', 'Researches Performed', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'ns', 'recycle', -3, 0, '', 'Structures Recycled', '1', '0', '0', '0');


#
# Teams
#
INSERT INTO hlstats_Teams
VALUES (NULL, 'ns', 'alien1team', 'Aliens', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'ns', 'marine1team', 'Marines', '0');


#
# Roles
#
INSERT INTO hlstats_Roles
VALUES (NULL, 'ns', 'soldier', 'Soldier', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'ns', 'commander', 'Commander', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'ns', 'skulk', 'Skulk', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'ns', 'gorge', 'Gorge', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'ns', 'lerk', 'Lerk', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'ns', 'fade', 'Fade', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'ns', 'onos', 'Onos', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'ns', 'gestate', 'Gestate', '1');


#
# Weapons
#
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'welder', 'Marine Welder', '3.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'item_mine', 'Marine Mine', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'handgrenade', 'Marine Hand Grenade', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'grenade', 'Marine Grenade Launcher', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'knife', 'Marine Knife', '4.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'pistol', 'Marine Pistol', '2.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'machinegun', 'Marine Light Machine Gun', '1.25');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'shotgun', 'Marine Shotgun', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'heavymachinegun', 'Marine Heavy Machine Gun', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'turret', 'Marine Turret', '.75');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'siegeturret', 'Marine Siege Turret', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'resourcetower', 'Electrified Marine Resource Tower', '2.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'team_turretfactor', 'Electric Marine Turret Factory', '2.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'team_advturretfactor', 'Electrified Marine Advance Turret Factory', '2.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'acidrocket', 'Fade Acid Rocket', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'bitegun', 'Skulk Bite', '1.25');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'charge', 'Onos Charge', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'claws', 'Onos Gore', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'divinewind', 'Skulk Xenocide', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'leap', 'Skulk Leap', '2.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'bite2gun', 'Lerk Bite', '2.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'spitgunspit', 'Gorge Spit', '2.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'sporegunprojectile', 'Lerk Spores', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'swipe', 'Fade Slash', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'healingspray', 'Gorge Health Spray', '3.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'parasite', 'Skulk Parasite', '3.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'devour', 'Onos Devour', '2.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ns', 'offensechamber', 'Offense Chamber', '1.00');


# Day of Defeat 1.0

#
# Game Definition
#
INSERT INTO hlstats_Games
VALUES ('dod', 'Day Of Defeat', '0');


#
# Awards
#
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dod', 'amerknife', 'Backstabbing Beotch', 'kills with the American Knife', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dod', 'luger', 'Luger Freak', 'kills with the Luger 08 Pistol', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dod', 'kar', 'KarMeister', 'kills with the Mauser Kar 98k', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dod', 'mp40', 'MP40 Hor', 'kills with the MP40 Machine Pistol', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dod', 'spade', 'Shovel God', 'kills with the spade', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dod', 'mp44', 'MP44 Hor', 'kills with the MP44 Assault Rifle', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dod', 'colt', 'Colt Freak', 'kills with the Colt .45 model 1911', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dod', 'garand', 'GarandMeister', 'kills with the M1 Garand Rifle', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dod', 'thompson', 'Thompson Hor', 'kills with the Thompson Submachine Gun', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dod', 'spring', 'Spring Sniper', 'snipings with the Springfield 03 Rifle', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dod', 'bar', 'Bar Browning Hor', 'kills with the BAR Browning Automatic Rifle', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dod', 'grenade', 'McVeigh Alert', 'bombings with the Grenade', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dod', 'garandbutt', 'Headsmasher', 'kills with Garand Butt Stock', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dod', 'bazooka', 'Bazooka Joe', 'kills with the Bazooka', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dod', 'pschreck', 'Panzerschreck Hans', 'kills with the Panzerschreck', NULL, NULL);


#
# Actions
#
INSERT INTO hlstats_Actions
VALUES (NULL, 'dod', 'dod_control_point', 6, 1, '', 'Control Points Captured', '1', '0', '1', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'dod', 'dod_capture_area', 6, 1, '', 'Areas Captured', '1', '0', '1', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'dod', 'dod_object_goal', 4, 0, '', 'Objectives Achieved', '1', '0', '0', '0');


#
# Teams
#
INSERT INTO hlstats_Teams
VALUES (NULL, 'dod', 'Allied', 'Allied', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'dod', 'Axis', 'Axis', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'dod', 'Spectators', 'Spectators', '0');


#
# Roles
#
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', 'Random', 'Random', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_allied_garand', 'American Rifleman', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_allied_carbine', 'American Staff Sergeant', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_allied_thompson', 'American Master Sergeant', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_allied_grease', 'American Sergeant', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_allied_sniper', 'American Sniper', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_allied_heavy', 'American Support Infantry', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_allied_mg', 'American Machine Gunner', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_alliedpara_garand', 'American Para Rifleman', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_alliedpara_carbine', 'American Para Staff Sergeant', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_alliedpara_thompson', 'American Para Master Sergeant', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_alliedpara_grease', 'American Para Sergeant', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_alliedpara_spring', 'American Para Sniper', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_alliedpara_bar', 'American Para Support Infantry', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_alliedpara_30cal', 'American Para Machine Gunner', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_brit_light', 'British Rifleman', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_brit_medium', 'British Sergeant Major', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_brit_sniper', 'British Marksman', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_brit_heavy', 'British Gunner', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_axis_kar98', 'Axis Grenadier', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_axis_k43', 'Axis Stosstruppe', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_axis_mp40', 'Axis Unteroffizier', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_axis_mp44', 'Axis Sturmtruppe', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_axis_sniper', 'Axis Scharfschütze', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_axis_mg34', 'Axis MG34-Schütze', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_axis_mg42', 'Axis MG42-Schütze', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_axispara_kar98', 'Axis Para Grenadier', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_axispara_k43', 'Axis Para Stosstruppe', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_axispara_mp40', 'Axis Para Unteroffizier', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_axispara_mp44', 'Axis Para Sturmtruppe', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_axispara_scopedkar', 'Axis Para Scharfschütze', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_axispara_fg42bipod', 'Axis Para Fg42-Zweinbein', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_axispara_fg42scope', 'Axis Para Fg42-Zielfernrohr', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_axispara_mg34', 'Axis Para MG34-Schütze', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'dod', '#class_axispara_mg42', 'Axis Para MG42-Schütze', '0');


#
# Weapons
#
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'k43', 'Karbiner 43', '1.50');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'luger', 'Luger 08 Pistol', '1.50');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'kar', 'Mauser Kar 98k', '1.30');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'mp40', 'MP40 Machine Pistol', '1.25');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'scopedkar', 'Mauser Karbiner 98k Sniper Rifle', '1.50');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'mp44', 'MP44 Assault Rifle', '1.35');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'colt', 'Colt .45 model 1911', '1.60');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'garand', 'M1 Garand Rifle', '1.30');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'thompson', 'Thompson Submachine Gun', '1.25');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'spring', 'Springfield Rifle with Scope', '1.50');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'bar', 'BAR Browning Automatic Rifle', '1.20');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'grenade', 'U.S. Grenade', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'enf_bayonet', 'Enfield Bayonet', '2.50');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'bren', 'Bren Machine Gun', '1.25');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'm1carbine', 'M1 Carbine', '1.20');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'greasegun', 'Greasegun', '1.30');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', '30cal', '.30 Caliber Machine Gun', '1.25');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'mg42', 'MG42 Machine Gun', '1.20');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'grenade2', 'German Grenade', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'spade', 'Spade Entrenchment Tool', '3.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'gerknife', 'Gravity Knife', '3.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'fg42', 'FG42 Paratroop Rifle', '1.25');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'world', 'worldspawn', '0.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'amerknife', 'U.S. Issue Knife', '3.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'bayonet', 'Karbiner Bayonet', '2.40');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'mg34', 'MG34 Machine Gun', '1.20');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'brit_knife', 'British Knife', '3.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'mortar', 'Mortar', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'fcarbine', 'F1 Carbine', '1.35');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'scoped_fg42', 'Scoped FG42', '1.30');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'bazooka', 'Bazooka', '2.25');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'enfield', 'Enfield Rifle', '1.35');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'garandbutt', 'Butt Stock Hit', '3.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'mills_bomb', 'British Grenade', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'piat', 'Piat', '2.25');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'pschreck', 'Panzerschreck', '2.25');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'scoped_enfield', 'Scoped Enfield', '1.50');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'sten', 'Sten Submachine Gun', '1.25');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dod', 'webley', 'Webley Revolver', '1.60');


# CS 1.6

INSERT INTO hlstats_Games
VALUES ('cstrike', 'Counter-Strike', '0');


#
# Awards
#
INSERT INTO hlstats_Awards
VALUES (NULL, 'O', 'cstrike', 'Defused_The_Bomb', 'Top Defuser', 'bomb defusions', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'O', 'cstrike', 'Planted_The_Bomb', 'Top Demolitionist', 'bomb plantings', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'O', 'cstrike', 'Rescued_A_Hostage', 'Top Hostage Rescuer', 'hostages rescued', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'O', 'cstrike', 'Assassinated_The_VIP', 'Top Assassin', 'assassinations', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'cstrike', 'elite', 'Dual Berretta Elites', 'kills with elite', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'cstrike', 'knife', 'Knife Maniac', 'knifings', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'cstrike', 'awp', 'AWP', 'snipings with awp', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'cstrike', 'p90', 'P90', 'kills with p90', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'cstrike', 'deagle', 'Desert Eagle', 'kills with deagle', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'cstrike', 'm3', 'Shotgun', 'kills with m3 shotgun', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'cstrike', 'usp', 'USP Master', 'kills with usp', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'cstrike', 'm4a1', 'Colt M4A1 Carbine', 'kills with m4a1', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'cstrike', 'glock18', 'Glock', 'kills with glock18', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'cstrike', 'ak47', 'AK47', 'kills with ak47', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'cstrike', 'famas', 'Fusil Automatique', 'kills with famas', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'cstrike', 'galil', 'Galil', 'kills with galil', NULL, NULL);


#
# Player Actions
#
INSERT INTO hlstats_Actions
VALUES (NULL, 'cstrike', 'Begin_Bomb_Defuse_Without_Kit', 0, 0, 'CT', 'Start Defusing the Bomb Without a Defuse Kit', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'cstrike', 'Begin_Bomb_Defuse_With_Kit', 0, 0, 'CT', 'Start Defusing the Bomb With a Defuse Kit', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'cstrike', 'Assassinated_The_VIP', 10, 0, 'TERRORIST', 'Assassinate the VIP', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'cstrike', 'Planted_The_Bomb', 15, 2, 'TERRORIST', 'Plant the Bomb', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'cstrike', 'Defused_The_Bomb', 15, 0, 'CT', 'Defuse the Bomb', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'cstrike', 'Touched_A_Hostage', 2, 0, 'CT', 'Touch a Hostage', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'cstrike', 'Rescued_A_Hostage', 5, 1, 'CT', 'Rescue a Hostage', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'cstrike', 'Killed_A_Hostage', -25, 1, 'CT', 'Kill a Hostage', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'cstrike', 'Became_VIP', 1, 0, 'CT', 'Become the VIP', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'cstrike', 'Spawned_With_The_Bomb', 2, 0, 'TERRORIST', 'Spawn with the Bomb', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'cstrike', 'Got_The_Bomb', 2, 0, 'TERRORIST', 'Pick up the Bomb', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'cstrike', 'Dropped_The_Bomb', -2, 0, 'TERRORIST', 'Drop the Bomb', '1', '0', '0', '0');


#
# Team Actions
#
INSERT INTO hlstats_Actions
VALUES (NULL, 'cstrike', 'CTs_Win', 0, 2, 'CT', 'All Terrorists eliminated', '0', '0', '1', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'cstrike', 'Terrorists_Win', 0, 2, 'TERRORIST', 'All Counter-Terrorists eliminated', '0', '0', '1', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'cstrike', 'All_Hostages_Rescued', 0, 10, 'CT', 'Counter-Terrorists rescued all the hostages', '0', '0', '1', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'cstrike', 'Target_Bombed', 0, 10, 'TERRORIST', 'Terrorists bombed the target', '0', '0', '1', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'cstrike', 'VIP_Assassinated', 0, 6, 'TERRORIST', 'Terrorists assassinated the VIP', '0', '0', '1', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'cstrike', 'Bomb_Defused', 0, 6, 'CT', 'Counter-Terrorists defused the bomb', '0', '0', '1', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'cstrike', 'VIP_Escaped', 0, 10, 'CT', 'VIP escaped', '0', '0', '1', '0');


#
# Teams
#
INSERT INTO hlstats_Teams
VALUES (NULL, 'cstrike', 'TERRORIST', 'Terrorist', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'cstrike', 'CT', 'Counter-Terrorist', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'cstrike', 'SPECTATOR', 'Spectator', '0');


#
# Weapons
#
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'knife', 'Bundeswehr Advanced Combat Knife', 1.80);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'usp', 'H&K USP .45 Tactical', 1.50);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'glock18', 'Glock 18 Select Fire', 1.50);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'deagle', 'Desert Eagle .50AE', 1.50);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'p228', 'Sig Sauer P-228', 1.50);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'm3', 'Benelli M3 Super 90 Combat', 1.40);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'xm1014', 'Benelli/H&K M4 Super 90 XM1014', 1.40);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'mp5navy', 'H&K MP5-Navy', 1.25);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'tmp', 'Steyr Tactical Machine Pistol', 1.25);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'p90', 'FN P90', 1.25);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'm4a1', 'Colt M4A1 Carbine', 1.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'ak47', 'Kalashnikov AK-47', 1.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'sg552', 'Sig Sauer SG-552 Commando', 1.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'scout', 'Steyr Scout', 1.60);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'awp', 'Arctic Warfare Magnum (Police)', 1.40);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'g3sg1', 'H&K G3/SG1 Sniper Rifle', 1.40);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'm249', 'M249 PARA Light Machine Gun', 0.80);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'grenade', 'High Explosive Grenade', 1.80);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'elite', 'Dual Beretta 96G Elite', 1.50);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'aug', 'Steyr Aug', 1.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'mac10', 'Ingram MAC-10', 1.25);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'fiveseven', 'FN Five-Seven', 1.50);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'ump45', 'H&K UMP45', 1.25);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'sg550', 'Sig SG-550 Sniper', 1.70);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'famas', 'Fusil Automatique', 1.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'cstrike', 'galil', 'Galil', 1.00);


# HalfLife Deathmatch

#
# Game Definition
#
INSERT INTO hlstats_Games
VALUES ('valve', 'Half-Life Deathmatch', '0');


#
# Awards
#
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'valve', '357', '357', 'kills with 357', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'valve', '9mmAR', 'MP5', 'kills with 9mmAR', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'valve', '9mmhandgun', 'Glock', 'kills with 9mmhandgun', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'valve', 'bolt', 'Crossbow Sniper', 'kills with bolt', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'valve', 'crowbar', 'Crowbar Maniac', 'murders with crowbar', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'valve', 'gluon gun', 'Gauss King', 'kills with gluon gun', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'valve', 'tau_cannon', 'Egon', 'kills with tau_cannon', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'valve', 'grenade', 'Grenadier', 'kills with grenade', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'valve', 'hornet', 'Hornet Master', 'kills with hornet', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'valve', 'rpg_rocket', 'Rocketeer', 'kills with rocket', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'valve', 'satchel', 'Lord Satchel', 'kills with satchel', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'valve', 'shotgun', 'Redneck', 'kills with shotgun', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'valve', 'snark', 'Snark Master', 'kills with snark', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'valve', 'tripmine', 'Shady Assassin', 'kills with tripmine', NULL, NULL);


#
# Weapons
#
INSERT INTO hlstats_Weapons
VALUES (NULL, 'valve', '357', '357 Revolver', 1.60);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'valve', '9mmAR', '9mm Automatic Rifle', 1.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'valve', '9mmhandgun', '9mm Handgun', 1.50);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'valve', 'bolt', 'Crossbow Bolt', 1.70);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'valve', 'crossbow', 'Crossbow', 1.40);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'valve', 'crowbar', 'Crowbar', 1.90);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'valve', 'tau_cannon', 'Egon Tau Cannon / Rail Gun', 1.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'valve', 'gluon gun', 'Gluon / Gauss Gun', 1.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'valve', 'grenade', 'Grenade', 1.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'valve', 'hornet', 'Hornet', 1.30);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'valve', 'rpg_rocket', 'Rocket Propelled Grenade', 1.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'valve', 'satchel', 'Satchel Charge', 1.50);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'valve', 'shotgun', 'Shotgun', 1.20);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'valve', 'snark', 'Snark', 1.80);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'valve', 'tripmine', 'Trip Mine', 1.60);


# Team Fortress 1.5

#
# Game Definition
#
INSERT INTO hlstats_Games
VALUES ('tfc', 'Team Fortress', '0');


#
# Awards
#
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'tfc', 'axe', 'Crowbar Maniac', 'murders with crowbar', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'tfc', 'spanner', 'Evil Engie', 'bludgeonings with spanner', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'tfc', 'rocket', 'Rocketeer', 'kills with rocket', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'tfc', 'ac', 'HWGuy Extraordinaire', 'ownings with ac', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'tfc', 'sniperrifle', 'Red Dot Special', 'snipings', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'tfc', 'flames', 'Fire Man', 'roastings', NULL, NULL);


#
# Player Actions
#
INSERT INTO hlstats_Actions
VALUES (NULL, 'tfc', 'rock2_goalitem', 5, 0, '', '(rock2) Pick up keycard', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'tfc', 'rock2_bcave1', 10, 3, '1', '(rock2) Blow Red Cave', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'tfc', 'rock2_rcave1', 10, 3, '2', '(rock2) Blow Blue Cave', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'tfc', 'rock2_rholedet', 10, 3, '2', '(rock2) Blow Blue Yard', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'tfc', 'rock2_bholedet', 10, 3, '1', '(rock2) Blow Red Yard', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'tfc', 'team two dropoff', 75, 25, '2', 'Captured Blue Flag', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'tfc', 'team one dropoff', 75, 25, '1', 'Captured Red Flag', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'tfc', 'Team 2 dropoff', 75, 25, '2', 'Captured Blue Flag', '1', '0', '0', '0');
INSERT INTO hlstats_Actions
VALUES (NULL, 'tfc', 'Team 1 dropoff', 75, 25, '1', 'Captured Red Flag', '1', '0', '0', '0');


#
# Teams
#
INSERT INTO hlstats_Teams
VALUES (NULL, 'tfc', '1', 'Blue', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'tfc', '2', 'Red', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'tfc', '3', 'Yellow', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'tfc', '4', 'Green', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'tfc', '#Hunted_team1', '(Hunted) VIP', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'tfc', '#Hunted_team2', '(Hunted) Bodyguards', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'tfc', '#Hunted_team3', '(Hunted) Assassins', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'tfc', '#Dustbowl_team1', 'Attackers', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'tfc', '#Dustbowl_team2', 'Defenders', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'tfc', 'SPECTATOR', 'Spectator', '0');


#
# Roles
#
INSERT INTO hlstats_Roles
VALUES (NULL, 'tfc', 'Scout', 'Scout', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'tfc', 'Sniper', 'Sniper', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'tfc', 'Soldier', 'Soldier', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'tfc', 'Demoman', 'Demoman', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'tfc', 'Medic', 'Medic', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'tfc', 'HWGuy', 'HWGuy', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'tfc', 'Pyro', 'Pyro', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'tfc', 'Spy', 'Spy', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'tfc', 'Engineer', 'Engineer', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'tfc', 'RandomPC', 'Random', '0');
INSERT INTO hlstats_Roles
VALUES (NULL, 'tfc', 'Civilian', 'The Hunted', '0');



#
# Weapons
#
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'sniperrifle', 'Sniper Rifle', 1.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'normalgrenade', 'Normal Grenade', 1.10);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'ac', 'Autocannon', 1.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'rocket', 'Rocket Launcher', 1.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'sentrygun', 'Sentry Gun', 1.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'supershotgun', 'Super Shotgun', 1.15);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'autorifle', 'Sniper Rifle (Auto Mode)', 1.20);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'empgrenade', 'EMP Grenade', 1.25);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'mirvgrenade', 'MIRV Grenade', 1.25);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'gl_grenade', 'Grenade Launcher', 1.35);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'pipebomb', 'Pipebomb', 1.35);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'timer', 'Infection Timer', 0.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'infection', 'Infection', 1.50);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'flames', 'Flame Thrower', 1.60);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'shotgun', 'Shotgun', 1.60);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'nails', 'Nail Gun', 1.70);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'nailgrenade', 'Nail Grenade', 1.70);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'supernails', 'Super Nail Gun', 1.65);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'axe', 'Crowbar', 1.80);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'medikit', 'Medikit', 1.85);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'napalmgrenade', 'Napalm Grenade', 1.70);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'detpack', 'Detpack', 1.80);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'gasgrenade', 'Gas Grenade', 1.90);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'spanner', 'Spanner', 2.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'caltrop', 'Caltrops', 2.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'railgun', 'Rail Gun', 1.85);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'tfc', 'building_dispenser', 'Dispenser', 2.00);


# The Specialists 2.1

#
# Game Definition
#
INSERT INTO hlstats_Games
VALUES ('ts', 'The Specialists', '0');

#
# Awards
#
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'ts', 'ump', 'Terrorist', 'hostages executed', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'ts', 'Golden Colts', '007', 'victims that died with class', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'ts', 'Katana', 'Shanghai Kid', 'victims sliced and diced', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'ts', 'Kung Fu', 'Master', 'got handed a can of whoopsass', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'ts', 'Barrett M82A1', 'Sniper', 'unsuspecting victims', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'ts', 'Combat Knife', 'Rambo', 'shankings by Combat Knife', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'ts', 'Sawed-off', 'Buckshot master', 'killing by ass blasting', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'ts', 'M60E3', 'Capt.', 'mowed down victims', NULL, NULL);

#
# Teams
#
INSERT INTO hlstats_Teams
VALUES (NULL, 'ts', 'agent', 'Agent', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'ts', 'castor', 'Castor', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'ts', 'gordon', 'Gordon', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'ts', 'hitman', 'Hitman', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'ts', 'laurence', 'Laurence', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'ts', 'merc', 'Merc', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'ts', 'seal', 'Seal', '0');

#
# Weapons
#
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'AK47', 'AK47', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'Akimbo_Berettas', 'Akimbo Berettas', '1.50');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'Akimbo_Five-seveN', 'Akimbo Five-sevens', '1.50');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'Akimbo_Mini-Uzi', 'Dual Mini-uzis', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'Akimbo_MK23', 'Akimbo MK23', '1.50');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'Barrett_M82A1', 'Barret', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'BENELLI_M3', 'Benelli-M3', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'Combat_Knife', 'Combat Knife', '3.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'Desert_Eagle', 'Desert Eagle', '2.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'Five-seven', 'Five-Seven', '2.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'Glock-18', 'Glock-18', '2.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'Glock-20C', 'Glock-20c', '2.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'Golden_Colts', 'Golden Colts', '1.50');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'Katana', 'Katana', '2.50');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'Kung_Fu', 'Kung Fu', '3.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'M16A4', 'M16a4', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'M4A1', 'M4A1', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'M60E3', 'M60e3', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'm61_grenade', 'Frag Grenade', '3.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'Mini-Uzi', 'Mini-uzi', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'MOSSBERG_500', 'Mossberg 500', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'MP5K', 'MP5k', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'MP5SD', 'MP5SD', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'MP7-PDW', 'Mp7-pdq', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'Raging_Bull', 'Raging Bull', '2.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'Ruger-MK1', 'Ruger-mk1', '2.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'Sawed-off', 'Sawed-off Shotgun', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'Seal_Knife', 'Seal Knife', '3.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'SOCOM-MK23', 'Socom-MK23', '2.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'SPAS-12', 'Spas-12', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'STEYR-AUG', 'Steyr-aug', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'STEYR-TMP', 'Steyr-TMP', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'throwing_knife', 'Throwing Knifes', '3.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'ump', 'UMP45', '1.00');
INSERT INTO hlstats_Weapons
VALUES (NULL, 'ts', 'USAS-12', 'USAS-12', '1.00');


# Deathmatch Classic

#
# Game Definition
#
INSERT INTO hlstats_Games
VALUES ('dmc', 'DeathMatch Classic', '0');


#
# Awards
#
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dmc', 'axe', 'Axe Murderer', 'hackings', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dmc', 'shotgun', 'Load of Buckshot', 'kills with shotgun', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dmc', 'doubleshotgun', 'Twin Shotty', 'kills with double shotgun', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dmc', 'spike', 'NailGunner', 'nailings', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dmc', 'superspike', 'Das Perforatah', 'puncturings', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dmc', 'grenade', 'Pineapples!', 'kills with grenades', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dmc', 'missile', 'Rocket Rider', 'kills with rockets', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dmc', 'lightning', 'Electrician', 'zappings', NULL, NULL);
INSERT INTO hlstats_Awards
VALUES (NULL, 'W', 'dmc', 'teledeath', 'Mr. Timing', 'telefrags', NULL, NULL);

#
# Team Actions
#

#
# Teams
#
INSERT INTO hlstats_Teams
VALUES (NULL, 'dmc', 'RED', 'Red Team', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'dmc', 'BLUE', 'Blue Team', '0');
INSERT INTO hlstats_Teams
VALUES (NULL, 'dmc', 'SPECTATOR', 'Spectator', '0');

#
# Weapons
#
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dmc', 'axe', 'Axe', 3.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dmc', 'shotgun', 'Shotgun', 2.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dmc', 'doubleshotgun', 'Double-Barrelled Shotgun', 1.50);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dmc', 'spike', 'Nailgun', 2.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dmc', 'superspike', 'Perforator', 1.50);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dmc', 'grenade', 'Grenade Launcher', 1.30);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dmc', 'missile', 'Rocket Launcher', 1.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dmc', 'lightning', 'Thunderbolt', 1.00);
INSERT INTO hlstats_Weapons
VALUES (NULL, 'dmc', 'teledeath', 'Telefrag', 3.00);
