#
# HLstats.plib - Library of common functions for all HLstats Perl scripts
# http://sourceforge.net/projects/hlstats/
#
# Copyright (C) 2001 Simon Garner
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
#
$db_prefix = "cvsdev_";
#
# E001. Connect
#
sub doEvent_Connect
{ 
my ($playerId, $ipAddr) = @_;
my $desc;
my $player = $g_players{"$s_addr/$playerId"};
my $playerstr = &getPlayerInfoString($player, $playerId);
my $hostname;
my $hostgroup;
if ($player)
{
$hostname = &resolveIp($ipAddr);
$hostgroup = "";
if ($hostname)
{
$hostgroup = &getHostGroup($hostname);
}
if ($g_mode eq "LAN")
{
$player->set("uniqueid", $ipAddr);
}
&recordEvent(
"Connects", 0,
$player->get("playerid"),
$ipAddr,
$hostname,
$hostgroup
);
}
else
{
$desc = "(IGNORED) NOPLAYERINFO: ";
}
return $desc . $playerstr . " connected, address \"$ipAddr\","
. " hostname \"$hostname\", hostgroup \"$hostgroup\"";
}
#
# E002. Enter Game
#
sub doEvent_EnterGame
{
my ($playerId) = @_;
my $desc;
my $player = $g_players{"$s_addr/$playerId"};
my $playerstr = &getPlayerInfoString($player, $playerId);
if ($player)
{
&recordEvent(
"Entries", 0,
$player->get("playerid")
);
}
else
{
$desc = "(IGNORED) NOPLAYERINFO: ";
}
return $desc . $playerstr . " entered the game";
}
#
# E003. Disconnect
#
sub doEvent_Disconnect
{
my ($playerId) = @_;
my $desc;
my $player = $g_players{"$s_addr/$playerId"};
my $playerstr = &getPlayerInfoString($player, $playerId);
if ($player)
{
&recordEvent(
"Disconnects", 0,
$player->get("playerid")
);
$player->updateDB();
delete($g_players{"$s_addr/$playerId"});
}
else
{
$desc = "(IGNORED) NOPLAYERINFO: ";
}
return $desc . $playerstr . " disconnected";
}
#
# 004. Suicide
#
sub doEvent_Suicide
{ 
my ($playerId, $weapon) = @_;
my $desc;
my $player = $g_players{"$s_addr/$playerId"};
my $playerstr = &getPlayerInfoString($player, $playerId);
if ($player)
{
&recordEvent(
"Suicides", 0,
$player->get("playerid"),
$weapon
);
$player->increment("suicides");
$player->increment("skill", -5);
$player->updateDB();
}
else
{
$desc = "(IGNORED) NOPLAYERINFO: ";
}
return $desc . $playerstr . " committed suicide with \"$weapon\"";
}
#
# 005. Team Selection
#
sub doEvent_TeamSelection
{
my ($playerId, $team) = @_;
my $desc;
my $player = $g_players{"$s_addr/$playerId"};
my $playerstr = &getPlayerInfoString($player, $playerId);
if ($player)
{
$player->set("team", $team);
&recordEvent(
"ChangeTeam", 0,
$player->get("playerid"),
$player->get("team")
);
}
else
{
$desc = "(IGNORED) NOPLAYERINFO: ";
}
return $desc . $playerstr . " joined team \"$team\"";
}
#
# 006. Role Selection
#
sub doEvent_RoleSelection
{
my ($playerId, $role) = @_;
my $desc;
my $player = $g_players{"$s_addr/$playerId"};
my $playerstr = &getPlayerInfoString($player, $playerId);
if ($player)
{
$player->set("role", $role);
&recordEvent(
"ChangeRole", 0,
$player->get("playerid"),
$player->get("role")
);
}
else
{
$desc = "(IGNORED) NOPLAYERINFO: ";
}
return $desc . $playerstr . " changed role to \"$role\"";
}
#
# 007. Change Name
#
sub doEvent_ChangeName
{
my ($playerId, $newname) = @_;
my $desc;
my $player = $g_players{"$s_addr/$playerId"};
my $playerstr = &getPlayerInfoString($player, $playerId);
if ($player)
{
my $name = $player->get("name");
&recordEvent(
"ChangeName", 0,
$player->get("playerid"),
$name,
$newname
);
$player->set("name", $newname);
if ($g_mode eq "NameTrack")
{
$player->set("uniqueid", $newname);
&recordEvent(
"ChangeName", 0,
$player->get("playerid"),
$name,
$newname
);
}
$player->updateDB();
}
else
{
$desc = "(IGNORED) NOPLAYERINFO: ";
}
return $desc . $playerstr . " changed name to \"$newname\"";
}
#
# 008. Frag
#
sub doEvent_Frag
{
my ($killerId, $victimId, $weapon) = @_;
my $desc;
my $killer = $g_players{"$s_addr/$killerId"};
my $victim = $g_players{"$s_addr/$victimId"};
my $killerstr = &getPlayerInfoString($killer, $killerId);
my $victimstr = &getPlayerInfoString($victim, $victimId);
if ($g_servers{$s_addr}->{numplayers} < $g_minplayers)
{
$desc = "(IGNORED) NOTMINPLAYERS: ";
}
elsif ($killer && $victim)
{
if (!&sameTeam($killer->{team}, $victim->{team}))
{
# Frag
&recordEvent(
"Frags", 0,
$killer->get("playerid"),
$victim->get("playerid"),
$weapon
);
my ($killerskill, $victimskill) = &calcSkill(
$killer->get("skill"),
$victim->get("skill"),
$weapon
);
$killer->set("skill", $killerskill);
$victim->set("skill", $victimskill);
$killer->increment("kills");
$victim->increment("deaths");
$killer->updateDB();
$victim->updateDB();
}
else
{
# Teamkill
&recordEvent(
"Teamkills", 0,
$killer->get("playerid"),
$victim->get("playerid"),
$weapon
);
$killer->increment("skill", -60);
$killer->updateDB();
$desc = "TEAMKILL: ";
}
}
else
{
$desc = "(IGNORED) NOPLAYERINFO: ";
}
return $desc . $killerstr . " killed " . $victimstr . " with \"$weapon\"";
}
#
# 010. Player-Player Actions
#
sub doEvent_PlayerPlayerAction
{
my ($playerId, $victimId, $action) = @_;
my $desc;
my $player = $g_players{"$s_addr/$playerId"};
my $victim = $g_players{"$s_addr/$victimId"};
my $playerstr = &getPlayerInfoString($player, $playerId);
my $victimstr = &getPlayerInfoString($victim, $victimId);
if ($g_servers{$s_addr}->{numplayers} < $g_minplayers)
{
$desc = "(IGNORED) NOTMINPLAYERS: ";
}
elsif ($player && $victim)
{
my $query = "
SELECT
id,
reward_player,
reward_team,
team,
LENGTH(code) AS codelength
FROM
".$db_prefix."hlstats_Actions
WHERE
game='" . $g_servers{$s_addr}->{game} . "'
AND (
code='" . &quoteSQL($g_servers{$s_addr}->{map} . "_$action") . "'
OR code='" . &quoteSQL($action) . "'
)
AND for_PlayerPlayerActions='1'
ORDER BY
codelength DESC
LIMIT 1
";
my $result = &doQuery($query);
if ($result->rows == 1)
{
my ($actionid, $reward_player, $reward_team, $team) = $result->fetchrow_array;
$result->finish;
&recordEvent(
"PlayerPlayerActions", 0,
$player->get("playerid"),
$victim->get("playerid"),
$actionid,
$reward_player
);
$player->increment("skill", $reward_player) if ($reward_player != 0);
$player->updateDB();
if ($team && $reward_team != 0)
{
&rewardTeam($team, $reward_team, $actionid);
}
}
else
{
$desc = "(IGNORED) ";
}
}
else
{
$desc = "(IGNORED) NOPLAYERINFO: ";
}
return $desc . $playerstr . " triggered \"$action\" against " . $victimstr;
}
#
# 011. Player Objectives/Actions
#
sub doEvent_PlayerAction
{
my ($playerId, $action) = @_;
my $desc;
my $player = $g_players{"$s_addr/$playerId"};
my $playerstr = &getPlayerInfoString($player, $playerId);
if ($g_servers{$s_addr}->{numplayers} < $g_minplayers)
{
$desc = "(IGNORED) NOTMINPLAYERS: ";
}
elsif ($player)
{
my $query = "
SELECT
id,
reward_player,
reward_team,
team,
LENGTH(code) AS codelength
FROM
".$db_prefix."hlstats_Actions
WHERE
game='" . $g_servers{$s_addr}->{game} . "'
AND (
code='" . &quoteSQL($g_servers{$s_addr}->{map} . "_$action") . "'
OR code='" . &quoteSQL($action) . "'
)
AND for_PlayerActions='1'
ORDER BY
codelength DESC
LIMIT 1
";
my $result = &doQuery($query);
if ($result->rows == 1)
{
my ($actionid, $reward_player, $reward_team, $team) = $result->fetchrow_array;
$result->finish;
&recordEvent(
"PlayerActions", 0,
$player->get("playerid"),
$actionid,
$reward_player
);
$player->increment("skill", $reward_player) if ($reward_player != 0);
$player->updateDB();
if ($team && $reward_team != 0)
{
&rewardTeam($team, $reward_team, $actionid);
}
}
else
{
$desc = "(IGNORED) ";
}
}
else
{
$desc = "(IGNORED) NOPLAYERINFO: ";
}
return $desc . $playerstr . " triggered \"$action\"";
}
#
# 012. Team Objectives/Actions
#
sub doEvent_TeamAction
{
my ($team, $action) = @_;
my $desc;
if ($g_servers{$s_addr}->{numplayers} < $g_minplayers)
{
$desc = "(IGNORED) NOTMINPLAYERS: ";
}
else
{
my $query = "
SELECT
id,
reward_team,
team,
LENGTH(code) AS codelength
FROM
".$db_prefix."hlstats_Actions
WHERE
game='" . $g_servers{$s_addr}->{game} . "'
AND (
code='" . &quoteSQL($g_servers{$s_addr}->{map} . "_$action") . "'
OR code='" . &quoteSQL($action) . "'
)
AND for_TeamActions='1'
ORDER BY
codelength DESC
LIMIT 1
";
my $result = &doQuery($query);
if ($result->rows == 1)
{
my ($actionid, $reward_team, $actionteam) = $result->fetchrow_array;
$result->finish;
if ($actionteam && $reward_team != 0)
{
&rewardTeam($actionteam, $reward_team, $actionid);
}
}
else
{
$desc = "(IGNORED) ";
}
}
return $desc . "Team \"$team\" triggered \"$action\"";
}
#
# 013. World Objectives/Actions
#
sub doEvent_WorldAction
{
my ($action) = @_;
my $desc;
if ($g_servers{$s_addr}->{numplayers} < $g_minplayers)
{
$desc = "(IGNORED) NOTMINPLAYERS: ";
}
else
{
my $query = "
SELECT
id,
reward_team,
team,
LENGTH(code) AS codelength
FROM
".$db_prefix."hlstats_Actions
WHERE
game='" . $g_servers{$s_addr}->{game} . "'
AND (
code='" . &quoteSQL($g_servers{$s_addr}->{map} . "_$action") . "'
OR code='" . &quoteSQL($action) . "'
)
AND for_WorldActions='1'
ORDER BY
codelength DESC
LIMIT 1
";
my $result = &doQuery($query);
if ($result->rows == 1)
{
my ($actionid, $reward_team, $team) = $result->fetchrow_array;
$result->finish;
if ($team && $reward_team != 0)
{
&rewardTeam($team, $reward_team, $actionid);
}
}
else
{
$desc = "(IGNORED) ";
}
}
return $desc . "World triggered \"$action\"";
}
#
# 014. Chat
#
sub doEvent_Chat
{
my ($type, $playerId, $message) = @_;
my $player = $g_players{"$s_addr/$playerId"};
my $playerstr = &getPlayerInfoString($player, $playerId);
if ($message =~ /^\/set ([^ ]+) (.+)$/i)
{
my $set_field = lc($1);
my $set_value = $2;
if ($set_field eq "name" || $set_field eq "realname")
{
&updatePlayerProfile($player, "fullName", $set_value);
}
elsif ($set_field eq "email" || $set_field eq "e-mail")
{
&updatePlayerProfile($player, "email", $set_value);
}
elsif ($set_field eq "homepage" || $set_field eq "url")
{
&updatePlayerProfile($player, "homepage", $set_value);
}
elsif ($set_field eq "icq" || $set_field eq "uin")
{
&updatePlayerProfile($player, "icq", $set_value);
}
}
elsif ($message =~ /^\/hideranking$/i)
{
my $result = &doQuery("
SELECT
hideranking
FROM
".$db_prefix."hlstats_Players
WHERE
playerId='" . $player->get("playerid") . "'
");
my ($hideranking) = $result->fetchrow_array;
my $hidedesc = "";
if ($hideranking == 0)
{
$hideranking = 1;
$hidedesc = "HIDDEN from";
}
else
{
$hideranking = 0;
$hidedesc = "VISIBLE on";
}
my $playerName = &abbreviate($player->get("name"));
&doQuery("
UPDATE
".$db_prefix."hlstats_Players
SET
hideranking='$hideranking'
WHERE
playerId='" . $player->get("playerid") . "'
");
&rcon("say '$playerName' is now $hidedesc the rankings.");
}
return $playerstr . " $type \"$message\"";
}
#
# 015. Team Alliance
#
sub doEvent_TeamAlliance
{
my ($team, $allyteam) = @_;
return "Team \"$team\" formed alliance with team \"$allyteam\"";
}
#
# 016. Team Score Report
#
sub doEvent_TeamScoreReport
{
my ($team, $score, $numplayers) = @_;
return "Team \"$team\" scored \"$score\" with \"$numplayers\" players";
}
#
# 017. Server Cvars
#
sub doEvent_ServerCvar
{
my ($type, $var, $value) = @_;
if ($type eq "start")
{
return "Server cvars start";
}
elsif ($type eq "end")
{
return "Server cvars end";
}
elsif ($type eq "var")
{
return "Server cvar \"$var\" = \"$value\"";
}
else
{
return "Server cvars: $type";
}
}
#
# 018. Log File
#
sub doEvent_LogFile
{
my ($type) = @_;
if ($type eq "start")
{
return "Log file started";
}
elsif ($type eq "end")
{
return "Log file closed";
}
else
{
return "Log file: $type";
}
}
#
# 019. Map
#
sub doEvent_ChangeMap
{
my ($type, $newmap) = @_;
$g_servers{$s_addr}->{map} = $newmap;
&printNotice("Current map for server \"$s_addr\" is now \"" .
$g_servers{$s_addr}->{map} . "\"");
if ($type eq "loading")
{
return "Loading map \"$newmap\"";
}
elsif ($type eq "started")
{
return "Started map \"$newmap\"";
}
else
{
return "Map \"$newmap\": $type";
}
}
#
# 020. Rcon
#
sub doEvent_Rcon
{
my ($type, $command, $password, $remoteAddr) = @_;
my $ipAddr;
my $port;
if ($remoteAddr =~ /([\d.]+):(\d+)/)
{
$ipAddr = $1;
$port = $2;
}
if ($g_rcon_record && (!$g_rcon_ignoreself || ($ipAddr ne $s_peerhost && $ipAddr ne "127.0.0.1")) )
{
&recordEvent(
"Rcon", 0,
$type,
$ipAddr,
$password,
$command
);
}
else
{
$desc = "(IGNORED) ";
}
return $desc . "$type Rcon from \"$ipAddr\", pw \"$password\": \"$command\"";
}
#
# 021. Server Name
#
sub doEvent_ServerName
{
my ($name) = @_;
return "Server name is \"$name\"";
}
#
# 500. Admin
#
sub doEvent_Admin
{
my ($type, $message, $playerName) = @_;
&recordEvent(
"Admin", 0,
$type,
$message,
$playerName
);
return "\"$type\" (\"$playerName\") \"$message\"";
}
#
# 501. Statsme (weapon)
#
sub doEvent_Statsme
{
my ($playerId, $weapon, $shots, $hits, $headshots, $damage, $kills, $deaths) = @_;
my $desc;
my $player = $g_players{"$s_addr/$playerId"};
my $playerstr = &getPlayerInfoString($player, $playerId);
if ($player)
{
&recordEvent(
"Statsme", 0,
$player->get("playerid"),
$weapon,
$shots,
$hits,
$headshots,
$damage,
$kills,
$deaths
);
}
else
{
$desc = "(IGNORED) NOPLAYERINFO: ";
}
return $desc . $playerstr . " STATSME weaponstats (weapon \"$weapon\") (shots \"$shots\") (hits \"$hits\") (headshots \"$headshots\") (damage \"$damage\") (kills \"$kills\") (deaths \"$deaths\")";
}
#
# 502. Statsme (weapon2)
#
sub doEvent_Statsme2
{
my ($playerId, $weapon, $head, $chest, $stomach, $leftarm, $rightarm, $leftleg, $rightleg) = @_;
my $desc;
my $player = $g_players{"$s_addr/$playerId"};
my $playerstr = &getPlayerInfoString($player, $playerId);
if ($player)
{
&recordEvent(
"Statsme2", 0,
$player->get("playerid"),
$weapon,
$head,
$chest,
$stomach,
$leftarm,
$rightarm,
$leftleg,
$rightleg
);
}
else
{
$desc = "(IGNORED) NOPLAYERINFO: ";
}
return $desc . $playerstr . " STATSME weaponstats2 (weapon \"$weapon\") (head \"$head\") (chest \"$chest\") (stomach \"$stomach\") (leftarm \"$leftarm\") (rightarm \"$rightarm\") (leftleg \"$leftleg\") (rightleg \"$rightleg\")";
}
#
# 503. Statsme (latency)
#
sub doEvent_Statsme_Latency
{
my ($playerId, $ping) = @_;
my $desc;
my $player = $g_players{"$s_addr/$playerId"};
my $playerstr = &getPlayerInfoString($player, $playerId);
if ($player)
{
&recordEvent(
"StatsmeLatency", 0,
$player->get("playerid"),
$ping
);
}
else
{
$desc = "(IGNORED) NOPLAYERINFO: ";
}
return $desc . $playerstr . " STATSME average latency \"$ping\"";
}
#
# 504. Statsme (time)
#
sub doEvent_Statsme_Time
{
my ($playerId, $time) = @_;
my $desc;
my $player = $g_players{"$s_addr/$playerId"};
my $playerstr = &getPlayerInfoString($player, $playerId);
if ($player)
{
&recordEvent(
"StatsmeTime", 0,
$player->get("playerid"),
$time
);
}
else
{
$desc = "(IGNORED) NOPLAYERINFO: ";
}
return $desc . $playerstr . " STATSME connection time \"$time\"";
}
1;