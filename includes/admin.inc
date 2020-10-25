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
	
	$auth->userdata['acclevel'] = 100; //set until permission matrix is created
	
	class AdminTask
	{
		public $title = '';
		public $acclevel = 0;
		public $type = '';
		public $description = '';
		
		publicfunction __construct ($title, $acclevel, $type='general', $description='')
		{
			$this->title = $title;
			$this->acclevel = $acclevel;
			$this->type = $type;
			$this->description = $description;
		}
	}
	
	
	class EditList
	{
		public $columns;
		public $keycol;
		public $table;
		public $icon;
		public $showid;
		
		public $errors;
		public $newerror;
		
		publicfunction __construct ($keycol, $table, $icon, $showid=true)
		{
			$this->keycol = $keycol;
			$this->table = $table;
			$this->icon = $icon;
			$this->showid = $showid;
		}
		
		publicfunction update ()
		{
			global $_POST, $xoopsDB;
			
			$okcols = 0;
			foreach ($this->columns as $col)
			{
				$value = $_POST["new_$col->name"];
				
				if ($value != '')
				{
					if ($col->type == 'ipaddress' &&
						!ereg("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$", $value))
					{
						$this->errors[] = "Column '$col->title' requires a valid IP address for new row";
						$this->newerror = true;
						$okcols++;
					}
					else
					{
						if ($qcols) {$qcols .= ', ';}
						$qcols .= $col->name;
						
						if ($qvals) {$qvals .= ', ';}
						
						if ($col->type == 'password')
						{
							$qvals .= "PASSWORD('$value')";
						}
						else
						{
							$qvals .= "'$value'";
						}
						
						if ($col->type != 'select' && $col->type != 'hidden' && $value != $col->datasource)
							{$okcols++;}
					}
				}
				elseif ($col->required)
				{
					$this->errors[] = "Required column '$col->title' must have a value for new row";
					$this->newerror = true;
				}
			}
			
			if ($okcols > 0 && !$this->errors)
			{
				$xoopsDB->query("
					INSERT INTO
						$this->table
						(
							$qcols
						)
					VALUES
					(
						$qvals
					)",
					false
				);
				
				if ($xoopsDB->error())
				{
					$this->errors[] = 'DB Error: ' . $xoopsDB->error();
				}
			}
			elseif ($okcols == 0)
			{
				$this->errors = [];
				$this->newerror = false;
			}
			
			if (!is_array($_POST['rows']))
				{return true;}
			
			foreach ($_POST['rows'] as $row)
			{
				$row = stripslashes($row);
				
				if ($_POST[$row . '_delete'])
				{
					$xoopsDB->query("
						DELETE FROM
							$this->table
						WHERE
							$this->keycol='" . addslashes($row) . "'
					");
				}
				else
				{
					$rowerror = false;
					
					$query = "UPDATE $this->table SET ";
					$i=0;
					foreach ($this->columns as $col)
					{
						$value = $_POST[$row . '_' . $col->name];
						
						if ($value == '' && $col->required)
						{
							$this->errors[] = "Required column '$col->title' must have a value for row '$row'";
							$rowerror = true;
						}
						elseif ($col->type == 'ipaddress' &&
						!ereg("^[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}$", $value))
						{
							$this->errors[] = "Column '$col->title' requires a valid IP address for row '$row'";
							$rowerror = true;
						}
						
						if ($i > 0) {$query .= ', ';}
						
						$query .= $col->name . "='$value'";
						$i++;
					}
					$query .= " WHERE $this->keycol='" . addslashes($row) . "'";
					
					if (!$rowerror)
					{
						$xoopsDB->query($query);
					}
				}
			}
			
			if ($this->error())
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		
		publicfunction draw ($result)
		{
			global $g_options, $_POST, $xoopsDB;
?>
<table width="75%" border=0 cellspacing=0 cellpadding=0>

<tr valign="top">
	<td><table width="100%" border=0 cellspacing=1 cellpadding=4>
	
		<tr valign="bottom">
<?php
			echo '<td></td>';
			
			if ($this->showid)
			{
?>
			<td align="right"><?php echo '<font>';
				echo 'ID';
				echo '</font>';
?></td>
<?php
			}
			
			foreach ($this->columns as $col)
			{
				if ($col->type == 'hidden') {continue;}
				echo '<td><font>' . $col->title
					. "</font></td>\n";
			}
?>
			<td align="center"><?php echo '<font>';
				echo 'Delete';
				echo '</font>';
?></td>
		</tr>

<?php
			while (false !== ($rowdata = $xoopsDB->fetchArray($result)))
			{
				echo "\n<tr>\n";
				echo '<td align="center">';
				echo '<img src="' . $g_options['imgdir'] . "/$this->icon.gif\" width=16 height=16 border=0></td>\n";

				if ($this->showid)
				{
					echo '<td align="right">'
						. '<font>' . $rowdata[$this->keycol] . '</font>'
						. "</td>\n";
				}
				
				$this->drawfields($rowdata, false, false);
?>
<td align="center"><input type="checkbox" name="<?php echo $rowdata[$this->keycol]; ?>_delete" value="1"></td>
<?php echo "</tr>\n\n";
			}
?>

<tr> 
<?php
			echo "<td align=\"center\"><font>new</font></td>\n";
			
			if ($this->showid)
				{echo "<td align=\"right\"><font>&nbsp;</font></td>\n";}
			
			if ($this->newerror)
			{
				$this->drawfields($_POST, true, true);
			}
			else
			{
				$this->drawfields([], true);
			}
			
			echo "<td></td>\n";
?>
</tr>
		
		</table></td>
</tr>

</table><p>
<?php
		}
		
		
		publicfunction drawfields ($rowdata=[], $new=false, $stripslashes=false)
		{
			global $g_options, $xoopsDB;
			
			$i=0;
			foreach ($this->columns as $col)
			{
				if ($new)
				{
					$keyval = "new";
					$rowdata[$col->name] = $rowdata["new_$col->name"];
					if ($stripslashes) {$rowdata[$col->name] = stripslashes($rowdata[$col->name]);}
				}
				else
				{
					$keyval = $rowdata[$this->keycol];
					if ($stripslashes) {$keyval = stripslashes($keyval);}
				}
				
				if ($col->type != 'hidden')
				{
					echo '<td>';
				}
				
				if ($i == 0 && !$new)
				{
					echo '<input type="hidden" name="rows[]" value="' . htmlspecialchars($keyval, ENT_QUOTES | ENT_HTML5) . '">';
				}
				
				if ($col->maxlength < 1)
					{$col->maxlength = '';}
				
				switch ($col->type)
				{
					case 'select':
						unset($coldata);
						
						if (ereg(';', $col->datasource))
						{
							// for manual datasource in format "key/value;key/value" or "key;key"
							foreach (explode(';', $col->datasource) as $v)
							{
								if (ereg('/', $v))
								{
									[$a, $b] = explode('/', $v);
									$coldata[$a] = $b;
								}
								else
								{
									$coldata[$v] = $v;
								}
							}
						}
						else
						{
							// for SQL datasource in format "table.column/keycolumn/where"
							[$col_table, $col_col] = explode('.', $col->datasource);
							[$col_col, $col_key, $col_where] = explode('/', $col_col);
							if ($col_where) {$col_where = "WHERE $col_where";}
							$col_result = $xoopsDB->query("SELECT $col_key, $col_col FROM $col_table $col_where ORDER BY $col_key");
							$coldata = [];
							while (list($k, $v) = $xoopsDB->fetchRow($col_result))
							{
								$coldata[$k] = $v;
							}
						}
						
						if ($col->width) {$width = ' style="width:' . $col->width*5 . 'px"';}
						else {$width = '';}
						
						echo '<select name="' . $keyval . "_$col->name\"$width>\n";
						
						if (!$col->required)
						{
							echo "<option value=\"\">\n";
						}
						
						$gotcval = false;
						
						foreach ($coldata as $k=>$v)
						{
							if ($rowdata[$col->name] == $k)
							{
								$selected = ' selected';
								$gotcval = true;
							}
							else
							{
								$selected = '';
							}
							
							echo "<option value=\"$k\"$selected>$v\n";
						}
						
						if (!$gotcval)
						{
							echo '<option value="' . $rowdata[$col->name] . '" selected>'
								. $rowdata[$col->name] . "\n";
						}
						
						echo '</select>';
						break;
					
					case 'checkbox':
						$selectedval = '1';
						$value = $rowdata[$col->name];
						
						if ($value == $selectedval) {$selected = ' checked';}
						else {$selected = '';}
						
						echo '<center><input type="checkbox" name="' . $keyval
							. "_$col->name\" value=\"$selectedval\"$selected></center>";
						break;
					
					case 'hidden':
						echo '<input type="hidden" name="' . $keyval
							. "_$col->name\" value=\"" . htmlspecialchars($col->datasource, ENT_QUOTES | ENT_HTML5) . '">';
						break;
					
					default:
						if ($col->datasource != '' && !isset($rowdata[$col->name]))
							{$value = $col->datasource;}
						else
							{$value = $rowdata[$col->name];}
						
						echo '<input type="text" name="' . $keyval
							. "_$col->name\" size=$col->width "
							. 'value="' . htmlspecialchars($value, ENT_QUOTES | ENT_HTML5) . '" class="textbox"'
							. " maxlength=\"$col->maxlength\">";
				}
				
				if ($col->type != 'hidden')
				{
					echo "</td>\n";
				}
				
				$i++;
			}
		}
		
		publicfunction error()
		{
			if (is_array($this->errors))
			{
				return implode("<p>\n\n", $this->errors);
			}
			else
			{
				return false;
			}
		}
	}

class EditListColumn
{
    public $name;
    public $title;
    public $width;
    public $required;
    public $type;
    public $datasource;
    public $maxlength;

    public function __construct($name, $title, $width = 20, $required = false, $type = 'text', $datasource = '', $maxlength = 0)
    {
        $this->name       = $name;
        $this->title      = $title;
        $this->width      = $width;
        $this->required   = $required;
        $this->type       = $type;
        $this->datasource = $datasource;
        $this->maxlength  = (int) $maxlength;
    }
}

class PropertyPage
{
    public $table;
    public $keycol;
    public $keyval;
    public $propertygroups = [];

    public function __construct($table, $keycol, $keyval, $groups)
    {
        $this->table          = $table;
        $this->keycol         = $keycol;
        $this->keyval         = $keyval;
        $this->propertygroups = $groups;
    }

    public function draw($data)
    {
        foreach ($this->propertygroups as $group) {
            $group->draw($data);
        }
    }

    public function update()
    {
        global $_POST, $xoopsDB;

        $setstrings = [];
        foreach ($this->propertygroups as $group) {
            foreach ($group->properties as $prop) {
                $setstrings[] = $prop->name . "='" . $_POST[$prop->name] . "'";
            }
        }

        $xoopsDB->query(
            '
				UPDATE
					' . $this->table . '
				SET
					' . implode(",\n", $setstrings) . '
				WHERE
					' . $this->keycol . "='" . $this->keyval . "'
			"
        );
    }
}

class PropertyPage_Group
{
public $title = '';
public $properties = [];

public function __construct($title, $properties)
{
    $this->title      = $title;
    $this->properties = $properties;
}

public function draw ($data)
{
global $g_options;
?>
    <b><?php echo $this->title; ?></b><br>
    <table width="100%" border=0 cellspacing=0 cellpadding=0>

        <tr valign="top">
            <td>
                <table width="100%" border=0 cellspacing=1 cellpadding=4>
                    <?php
                    foreach ($this->properties as $prop) {
                        $prop->draw($data[$prop->name]);
                    }
                    ?>
                </table>
            </td>
        </tr>

    </table>
<p>
    <?php
    }
    }

    class PropertyPage_Property
{
    public $name;
    public $title;
    public $type;

    public function __construct($name, $title, $type, $datasource = '')
    {
        $this->name       = $name;
        $this->title      = $title;
        $this->type       = $type;
        $this->datasource = $datasource;
    }

    public function draw($value)
    {
        global $g_options;
        ?>
        <tr valign="middle">
            <td width="45%"><?php
                echo '<font class="fontNormal">';
                echo $this->title . ':';
                echo '</font>';
                ?></td>
            <td width="55%"><?php
                switch ($this->type) {
                    case 'textarea':
                        echo "<textarea name=\"$this->name\" cols=35 rows=4 wrap=\"virtual\">" . htmlspecialchars($value, ENT_QUOTES | ENT_HTML5) . '</textarea>';
                        break;

                    case 'select':
                        // for manual datasource in format "key/value;key/value" or "key;key"
                        foreach (explode(';', $this->datasource) as $v) {
                            if (ereg('/', $v)) {
                                [$a, $b] = explode('/', $v);
                                $coldata[$a] = $b;
                            } else {
                                $coldata[$v] = $v;
                            }
                        }

                        echo getSelect($this->name, $coldata, $value);
                        break;

                    default:
                        echo "<input type=\"text\" name=\"$this->name\" size=35 value=\"" . htmlspecialchars($value, ENT_QUOTES | ENT_HTML5) . '" class="textbox">';
                        break;
                }
                ?></td>
        </tr>
        <?php
    }
}
	
	
	function message ($icon, $msg)
	{
		global $g_options;
?>
		<table width="60%" border=0 cellspacing=0 cellpadding=0>
		
		<tr valign="top">
			<td width=40><img src="<?php echo $g_options['imgdir'] . "/$icon"; ?>.gif" width=16 height=16 border=0 hspace=5></td>
			<td width="100%"><?php
	echo '<font class="fontNormal">';
	echo "<b>$msg</b>";
	echo '</font>';
?></td>
		</tr>
		
		</table><p>
<?php
	}
	
	
	
	
	$selTask = $_GET['task'];
	$selGame = $_GET['admingame'];
?>

<table width="90%" align="center" border=0 cellspacing=0 cellpadding=0>

<tr valign="top">
	<td><?php
	echo '<font class="fontNormal">';
	
	// General Settings
	$admintasks['options']			= new AdminTask('HLstats Options', 100);
	$admintasks['games']			= new AdminTask('Games', 100);
	$admintasks['hostgroups']		= new AdminTask('Host Groups', 100);
	$admintasks['clantags']			= new AdminTask('Clan Tag Patterns', 80);
	$admintasks['plugins']			= new AdminTask('Server Plugins', 80);
	
	// Game Settings
	$admintasks['servers']			= new AdminTask('Servers', 100, 'game');
	$admintasks['actions']			= new AdminTask('Actions', 80, 'game');
	$admintasks['teams']			= new AdminTask('Teams', 80, 'game');
	$admintasks['roles']			= new AdminTask('Roles', 80, 'game');
	$admintasks['weapons']			= new AdminTask('Weapons', 80, 'game');
	$admintasks['awards_weapons']	= new AdminTask('Weapon Awards', 80, 'game');
	$admintasks['awards_actions']	= new AdminTask('Action Awards', 80, 'game');
	
	// Tools
	$admintasks['tools_editdetails'] = new AdminTask('Edit Player or Clan Details', 80, 'tool',
		"Edit a player or clan's profile information.");
	$admintasks['tools_adminevents'] = new AdminTask('Admin-Event History', 80, 'tool',
		'View event history of logged Rcon commands and Admin Mod messages.');
	$admintasks['tools_ipstats']	= new AdminTask('Host Statistics', 80, 'tool',
		'See which ISPs your players are using.');
	$admintasks['tools_optimize']	= new AdminTask('Optimize Database', 100, 'tool',
		'This operation tells the MySQL server to clean up the database tables, optimizing them for better performance. It is recommended that you run this at least once a month.');
	$admintasks['tools_reset']		= new AdminTask('Reset Statistics', 100, 'tool', 'Delete all players, clans and events from the database.');
	
	// Sub-Tools
	$admintasks['tools_editdetails_player'] = new AdminTask('Edit Player Details', 80, 'subtool', "Edit a player's profile information.");
	$admintasks['tools_editdetails_clan']   = new AdminTask('Edit Clan Details', 80, 'subtool', "Edit a clan's profile information.");
	
	// Show Tool
	if ($admintasks[$selTask] && $admintasks[$selTask]->type == 'tool' || $admintasks[$selTask]->type == 'subtool')
	{
		$task = $admintasks[$selTask];
		$code = $selTask;
?>
&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;<a href="<?php echo $g_options['admin_scripturl']; ?>?mode=admin">Tools</a></b><br>
<img src="<?php echo $g_options['imgdir']; ?>/spacer.gif" width=1 height=8 border=0><br>

<?php
		include INCLUDE_PATH . "/admintasks/$code.inc";
	}
	else
	{
		// General Settings
?>
&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;General Settings</b><p>
<?php
		foreach ($admintasks as $code=>$task)
		{
			if ($auth->userdata['acclevel'] >= $task->acclevel && $task->type == 'general')
			{
				if ($selTask == $code)
				{
?>
&nbsp;&nbsp;&nbsp;&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;<a href="<?php echo $g_options['admin_scripturl']; ?>?mode=admin" name="<?php echo $code; ?>"><?php echo $task->title; ?></a></b><p>

<form method="POST" action="<?php echo $g_options['admin_scripturl']; ?>?mode=admin&task=<?php echo $code; ?>#<?php echo $code; ?>">

<table width="100%" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="10%">&nbsp;</td>
	<td width="90%"><?php
	echo '<font class="fontNormal">';
	include INCLUDE_PATH . "/admintasks/$code.inc";
	echo '</font>';
?></td>
</tr>

</table><br><br>
</form>
<?php
				}
				else
				{
?>
&nbsp;&nbsp;&nbsp;&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/rightarrow.gif" width=6 height=9 border=0 align="middle" alt="rightarrow.gif"><b>&nbsp;<a href="<?php echo $g_options['admin_scripturl']; ?>?mode=admin&task=<?php echo $code; ?>#<?php echo $code; ?>"><?php echo $task->title; ?></a></b><p>
<?php
				}
			}
		}
		
		
		// Game Settings
?>
&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Game Settings</b><p>
<?php
		$gamesresult = $xoopsDB->query('
			SELECT
				name,
				code
			FROM
				'.$xoopsDB->prefix('hlstats_Games').'
		');
		
		while (false !== ($gamedata = $xoopsDB->fetchArray($gamesresult)))
		{
			$gamename = $gamedata['name'];
			$gamecode = $gamedata['code'];
			
			if ($gamecode == $selGame)
			{
?>
&nbsp;&nbsp;&nbsp;&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;<a href="<?php echo $g_options['admin_scripturl']; ?>?mode=admin" name="game_<?php echo $gamecode; ?>"><?php echo $gamename;?></a></b> (<?php echo $gamecode; ?>)<p>
<?php
				foreach ($admintasks as $code=>$task)
				{
					if ($auth->userdata['acclevel'] >= $task->acclevel && $task->type == 'game')
					{
						if ($selTask == $code)
						{
?>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;<a href="<?php echo $g_options['admin_scripturl']; ?>?mode=admin&admingame=<?php echo $gamecode; ?>" name="<?php echo $code; ?>"><?php echo $task->title; ?></a></b><p>

<form method="POST" action="<?php echo $g_options['admin_scripturl']; ?>?mode=admin&admingame=<?php echo $gamecode; ?>&task=<?php echo $code; ?>#<?php echo $code; ?>">

<table width="100%" border=0 cellspacing=0 cellpadding=0>

<tr>
	<td width="10%">&nbsp;</td>
	<td width="90%"><?php
	echo '<font class="fontNormal">';
	include INCLUDE_PATH . "/admintasks/$code.inc";
	echo '</font>';
?></td>
</tr>

</table><br><br>
</form>
<?php
						}
						else
						{
?>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/rightarrow.gif" width=6 height=9 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;<a href="<?php echo $g_options['admin_scripturl']; ?>?mode=admin&admingame=<?php echo $gamecode; ?>&task=<?php echo $code; ?>#<?php echo $code; ?>"><?php echo $task->title; ?></a></b><p>
<?php
						}
					}
				}
			}
			else
			{
?>
&nbsp;&nbsp;&nbsp;&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/rightarrow.gif" width=6 height=9 border=0 align="middle" alt="rightarrow.gif"><b>&nbsp;<a href="<?php echo $g_options['admin_scripturl']; ?>?mode=admin&admingame=<?php echo $gamecode; ?>#game_<?php echo $gamecode; ?>"><?php echo $gamename;?></a></b> (<?php echo $gamecode; ?>)<p>
<?php
			}
		}
	}
	echo "</td>\n";
	
	if (!$selTask || !$admintasks[$selTask])
	{
		echo '<td width="50%">';
		echo '<font class="fontNormal">';
?>
&nbsp;<img src="<?php echo $g_options['imgdir']; ?>/downarrow.gif" width=9 height=6 border=0 align="middle" alt="downarrow.gif"><b>&nbsp;Tools</b>

<ul>
<?php
	foreach ($admintasks as $code=>$task)
	{
		if ($auth->userdata['acclevel'] >= $task->acclevel && $task->type == 'tool')
		{
?>	<li><b><a href="<?php echo $g_options['admin_scripturl']; ?>?mode=admin&task=<?php echo $code; ?>"><?php echo $task->title; ?></a></b><br>
		<?php echo $task->description; ?><p>
<?php
		}
	}
?>
</ul>
<?php
		echo '</font>';
		echo '</td>';
	}
?>
</tr>

</table>
