<?php
	/*
	 * HLstats - Real-time player and clan rankings and statistics for Half-Life
	 * http://sourceforge.net/projects/hlstats/
	 *
	 * Copyright (C) 2001  Simon Garner
	 *
	 * This program is free software; you can redistribute it and/or
	 * modify it under the terms of the GNU General Public License
	 * as published by the Free Software Foundation; either version 2
	 * of the License, or (at your option) any later version.
	 *
	 * This program is distributed in the hope that it will be useful,
	 * but WITHOUT ANY WARRANTY; without even the implied warranty of
	 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	 * GNU General Public License for more details.
	 *
	 * You should have received a copy of the GNU General Public License
	 * along with this program; if not, write to the Free Software
	 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
	 */
	
	// Search Class

class Search
{
public $query;
public $type;
public $game;

public $uniqueid_string = 'Unique ID';
public $uniqueid_string_plural = 'Unique IDs';


public function __construct($query, $type, $game)
{
    $this->query = $query;
    $this->type  = $type;
    $this->game  = $game;

    if (MODE == 'LAN') {
        $this->uniqueid_string        = 'IP Address';
        $this->uniqueid_string_plural = 'IP Addresses';
    }
}

public function drawForm ($getvars = [], $searchtypes = -1)
{
global $g_options, $xoopsDB;

if (!is_array($searchtypes)) {
    $searchtypes = [
        'player'   => 'Player names',
        'uniqueid' => 'Player ' . $this->uniqueid_string_plural,
        'clan'     => 'Clan names',
    ];
}
?>

<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

    <tr valign="top">
        <td width="100%"><font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Find a Player or Clan</b></font>
            <p>

            <form method="GET" action="<?php
            if (ADMIN_FILES == 'true') {
                echo $g_options['admin_scripturl'];
            } else {
                echo $g_options['scripturl'];
            }
            ?>">
                <?php
                foreach ($getvars as $var => $value) {
                    echo "<input type=\"hidden\" name=\"$var\" value=\"$value\">\n";
                }
                ?>

                <table width="100%" border=0 cellspacing=0 cellpadding=0>

                    <tr>
                        <td width="10%">&nbsp;</td>
                        <td width="90%">
                            <table width="40%" border=0 cellspacing=0 cellpadding=0>

                                <tr valign="top">
                                    <td>
                                        <table width="100%" border=0 cellspacing=1 cellpadding=4>

                                            <tr valign="middle">
                                                <td nowrap width="45%"><font class="fontNormal">Search For:</font></td>
                                                <td width="55%"><input type="text" name="q" size=20 maxlength=128 value="<?php echo stripslashes($this->query); ?>" class="textbox"></td>
                                            </tr>

                                            <tr valign="middle">
                                                <td nowrap width="45%"><font class="fontNormal">In:</font></td>
                                                <td width="55%">
                                                    <?php
                                                    echo getSelect(
                                                        'st',
                                                        $searchtypes,
                                                        $this->type
                                                    );
                                                    ?></td>
                                            </tr>

                                            <tr valign="middle">
                                                <td nowrap width="45%"><font class="fontNormal">Game:</font></td>
                                                <td width="55%">
                                                    <?php
                                                    $games[''] = '(All)';

                                                    $sql = $xoopsDB->query('SELECT code, name FROM ' . $xoopsDB->prefix('hlstats_Games') . " WHERE hidden='0' ORDER BY name");
                                                    while (false !== ($var = $xoopsDB->fetchRow($sql))) {
                                                        $games[$var[0]] = $var[1];
                                                    }

                                                    echo getSelect('game', $games, $this->game);
                                                    ?></td>
                                            </tr>

                                        </table>
                                    </td>
                                    <td align="right">
                                        <table border=0 cellspacing=1 cellpadding=10 width="100%" height="100%">
                                            <tr>

                                                <td><input type="submit" value=" Find Now " class="submit"></td>
                                            </tr>
                                        </table>
                                    </td>
                                </tr>

                            </table>
                        </td>
                    </tr>

                </table>
            </form>
        </td>
    </tr>

</table><p>


    <?php
    }

    public function drawResults ($link_player = -1, $link_clan = -1)
    {
    global $g_options, $xoopsDB;

    if ($link_player == -1) {
        $link_player = 'mode=playerinfo&amp;player=%k';
    }
    if ($link_clan == -1) {
        $link_clan = 'mode=claninfo&amp;clan=%k';
    }
    ?>
<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

    <tr valign="top">
        <td width="100%"><a name="results"></a>
            <font class="fontNormal">&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Search Results</b></font></td>
    </tr>

</table><p>
    <?php
    $sr_query = ereg_replace(' ', '%', $this->query);

    if ($this->type == 'player') {
        $table = new Table(
            [
                new TableColumn(
                    'playerId', 'ID', 'width=5align=right'
                ),
                new TableColumn(
                    'name', 'Name', 'width=65&icon=player&link=' . urlencode($link_player)
                ),
                new TableColumn(
                    'gamename', 'Game', 'width=30'
                ),
            ], 'playerId', 'name', 'playerId', false, 50, 'page', 'sort', 'sortorder', 'results', 'asc'
        );

        if ($this->game) {
            $andgame = 'AND ' . $xoopsDB->prefix('hlstats_Players') . ".game='" . $this->game . "'";
        } else {
            $andgame = '';
        }

        $result = $xoopsDB->query(
            '
					SELECT
						' . $xoopsDB->prefix('hlstats_PlayerNames') . '.playerId,
						' . $xoopsDB->prefix('hlstats_PlayerNames') . '.name,
						' . $xoopsDB->prefix('hlstats_Games') . '.name AS gamename
					FROM
						' . $xoopsDB->prefix('hlstats_PlayerNames') . '
					LEFT JOIN ' . $xoopsDB->prefix('hlstats_Players') . ' ON
						' . $xoopsDB->prefix('hlstats_Players') . '.playerId = ' . $xoopsDB->prefix('hlstats_PlayerNames') . '.playerId
					LEFT JOIN ' . $xoopsDB->prefix('hlstats_Games') . ' ON
						' . $xoopsDB->prefix('hlstats_Games') . '.code = ' . $xoopsDB->prefix('hlstats_Players') . '.game
					WHERE
						' . $xoopsDB->prefix('hlstats_Games') . ".hidden='0' AND
						" . $xoopsDB->prefix('hlstats_PlayerNames') . ".name LIKE '%$sr_query%'
						$andgame
					ORDER BY
						$table->sort $table->sortorder,
						$table->sort2 $table->sortorder
					LIMIT $table->startitem,$table->numperpage
				"
        );

        $resultCount = $xoopsDB->query(
            '
					SELECT
						COUNT(*)
					FROM
						' . $xoopsDB->prefix('hlstats_PlayerNames') . '
					LEFT JOIN ' . $xoopsDB->prefix('hlstats_Players') . ' ON
						' . $xoopsDB->prefix('hlstats_Players') . '.playerId = ' . $xoopsDB->prefix('hlstats_PlayerNames') . '.playerId
					WHERE
						' . $xoopsDB->prefix('hlstats_PlayerNames') . ".name LIKE '%$sr_query%'
						$andgame
				"
        );

        [$numitems] = $xoopsDB->fetchRow($resultCount);

        $table->draw($result, $numitems, 90);
    } elseif ($this->type == 'uniqueid') {
        $table = new Table(
            [
                new TableColumn(
                    'uniqueId', $this->uniqueid_string, 'width=15&align=right'
                ),
                new TableColumn(
                    'lastName', 'Name', 'width=50&icon=player&link=' . urlencode($link_player)
                ),
                new TableColumn(
                    'gamename', 'Game', 'width=30'
                ),
                new TableColumn(
                    'playerId', 'ID', 'width=5&align=right'
                ),
            ], 'playerId', 'lastName', 'uniqueId', false, 50, 'page', 'sort', 'sortorder', 'results', 'asc'
        );

        if ($this->game) {
            $andgame = 'AND ' . $xoopsDB->prefix('hlstats_PlayerUniqueIds') . ".game='" . $this->game . "'";
        } else {
            $andgame = '';
        }

        $result = $xoopsDB->query(
            '
					SELECT
						' . $xoopsDB->prefix('hlstats_PlayerUniqueIds') . '.uniqueId,
						' . $xoopsDB->prefix('hlstats_PlayerUniqueIds') . '.playerId,
						' . $xoopsDB->prefix('hlstats_Players') . '.lastName,
						' . $xoopsDB->prefix('hlstats_Games') . '.name AS gamename
					FROM
						' . $xoopsDB->prefix('hlstats_PlayerUniqueIds') . '
					LEFT JOIN ' . $xoopsDB->prefix('hlstats_Players') . ' ON
						' . $xoopsDB->prefix('hlstats_Players') . '.playerId = ' . $xoopsDB->prefix('hlstats_PlayerUniqueIds') . '.playerId
					LEFT JOIN ' . $xoopsDB->prefix('hlstats_Games') . ' ON
						' . $xoopsDB->prefix('hlstats_Games') . '.code = ' . $xoopsDB->prefix('hlstats_PlayerUniqueIds') . '.game
					WHERE
						' . $xoopsDB->prefix('hlstats_Games') . ".hidden='0' AND
						" . $xoopsDB->prefix('hlstats_PlayerUniqueIds') . ".uniqueId LIKE '%$sr_query%'
						$andgame
					ORDER BY
						$table->sort $table->sortorder,
						$table->sort2 $table->sortorder
					LIMIT $table->startitem,$table->numperpage
				"
        );

        $resultCount = $xoopsDB->query(
            '
					SELECT
						COUNT(*)
					FROM
						' . $xoopsDB->prefix('hlstats_PlayerUniqueIds') . '
					LEFT JOIN ' . $xoopsDB->prefix('hlstats_Players') . ' ON
						' . $xoopsDB->prefix('hlstats_Players') . '.playerId = ' . $xoopsDB->prefix('hlstats_PlayerUniqueIds') . '.playerId
					WHERE
						' . $xoopsDB->prefix('hlstats_PlayerUniqueIds') . ".uniqueId LIKE '%$sr_query%'
						$andgame
				"
        );

        [$numitems] = $xoopsDB->fetchRow($resultCount);

        $table->draw($result, $numitems, 90);
    } elseif ($this->type == 'clan') {
        $table = new Table(
            [
                new TableColumn(
                    'tag', 'Tag', 'width=15'
                ),
                new TableColumn(
                    'name', 'Name', 'width=50&icon=clan&link=' . urlencode($link_clan)
                ),
                new TableColumn(
                    'gamename', 'Game', 'width=30'
                ),
                new TableColumn(
                    'clanId', 'ID', 'width=5&align=right'
                ),
            ], 'clanId', 'name', 'tag', false, 50, 'page', 'sort', 'sortorder', 'results', 'asc'
        );

        if ($this->game) {
            $andgame = 'AND ' . $xoopsDB->prefix('hlstats_Clans') . ".game='" . $this->game . "'";
        } else {
            $andgame = '';
        }

        $result = $xoopsDB->query(
            '
					SELECT
						' . $xoopsDB->prefix('hlstats_Clans') . '.clanId,
						' . $xoopsDB->prefix('hlstats_Clans') . '.tag,
						' . $xoopsDB->prefix('hlstats_Clans') . '.name,
						' . $xoopsDB->prefix('hlstats_Games') . '.name AS gamename
					FROM
						' . $xoopsDB->prefix('hlstats_Clans') . '
					LEFT JOIN ' . $xoopsDB->prefix('hlstats_Games') . ' ON
						' . $xoopsDB->prefix('hlstats_Games') . '.code = ' . $xoopsDB->prefix('hlstats_Clans') . '.game
					WHERE
						' . $xoopsDB->prefix('hlstats_Games') . ".hidden='0' AND
						(
							" . $xoopsDB->prefix('hlstats_Clans') . ".tag LIKE '%$sr_query%'
							OR " . $xoopsDB->prefix('hlstats_Clans') . ".name LIKE '%$sr_query%'
						)
						$andgame
					ORDER BY
						$table->sort $table->sortorder,
						$table->sort2 $table->sortorder
					LIMIT $table->startitem,$table->numperpage
				"
        );

        $resultCount = $xoopsDB->query(
            '
					SELECT
						COUNT(*)
					FROM
						' . $xoopsDB->prefix('hlstats_Clans') . "
					WHERE
						(
							tag LIKE '%$sr_query%'
							OR name LIKE '%$sr_query%'
						)
						$andgame
				"
        );

        [$numitems] = $xoopsDB->fetchRow($resultCount);

        $table->draw($result, $numitems, 90);
    }

    echo '<p><center>' . '<font class="fontNormal">' . "Search results: <b>$numitems</b> items matching \"" . htmlspecialchars($sr_query, ENT_QUOTES | ENT_HTML5) . '".' . '</font>' . '</center>';
    }
    }
    ?>
