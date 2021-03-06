<?
if(!isset($db_inc_php))
{
$db_inc_php=1;

class db
{
	var $dbuser,$dbname,$dbpass,$dbhost;
	function db($user="",$pw="",$name="",$host="")
	{
		global $dbhost,$dbname,$dbuser,$dbpass;
		if($user=="")
			$this->dbuser=$dbuser;
		else
			$this->dbuser=$user;
			
		if($host=="")
			$this->dbhost=$dbhost;
		else
			$this->dbhost=$host;
			
		if($pw=="")
			$this->dbpass=$dbpass;
		else
			$this->dbpass=$pw;
			
		if($name=="")
			$this->dbname=$dbname;
		else
			$this->dbname=$name;
	}
	function get()
	{
		if(!isset($this->connection))
		{
			if ($this->connection = mysql_connect($this->dbhost, $this->dbuser, $this->dbpass))
				{
					if (!mysql_select_db($this->dbname))
					{
						fatal_error('db::get(): cannot select '.$this->name);
						return false;
					}
				} 
			else 
				{
					fatal_error('db::get(): connect connect tohost '.$dbhost);
					return false;
				}
		}
		return $this->connection;
	}
	function getRowById($table, $id)
	{
		$r=$this->query("select * from $table where id=$id;");
		if($this->num_rows($r)==0)
			return array();
		$row=$this->result($r);
		return $row;
	}

	function getIds($table,$sort=array())
	{
		$q="select id from $table";
		$sorting=false;
		foreach($sort as $s)
		{
			$s=$this->save_string($s);
			if($s=="")
				return array();
			if(!$sorting)
				{
					$sorting=true;
					$q.=" order by";
				}
			else
				$q.=",";
			$q.=" $s";
		}

		$q.=";";
		$r=$this->query($q);
		$a=array();
		for($i=0;$i<$this->num_rows($r);$i++)
		{
			$row=$this->result($r);
			$a[]=$row[id];
		}
		return $a;
	}
	
	function query($query)
	{
		$link = $this->get();
		if ($link)
		{
			if ($result = mysql_query($query, $link))
			{
				if (!strncasecmp("insert", $query, 6))
				{
					$this->resID = mysql_insert_id($link);
				}
				return $result;
			} else
			{
				if ($this->error() != "")
					error('db::query(): query '.$query.' failed. <br>error: '.$this->error());
				return false;
			}
		}
	}
	function errno()
	{
		return mysql_errno();
	}

	function error()
	{
		if ($this->errno() != 0)
		{
			return '#'.$this->errno().' ('.mysql_error().')';
		}
		else
		{
			return '';
		}
	}
		function result_id()
		{
			return $this->resID;
		}

		function result($query_result)
		{
			$r = mysql_fetch_assoc($query_result);
			if ($this->errno() != 0)
			{
				error('[mysql.inc.php] db->result(): Ergebnisabfrage schlug fehl.<br>MySQL Fehler: '.$this->error());
				return false;
			} else {
				return $r;
			}
		}

		function result_list($query_result)
		{
			$r = mysql_fetch_row($query_result);
			if ($this->errno() != 0)
			{
				error('[mysql.inc.php] db->result_list(): Ergebnisabfrage schlug fehl.<br>MySQL Fehler: '.$this->error());			
				return false;
			} else {
				return $r;
			}
		}

		function insert_id()
		{
			return mysql_insert_id();
		}
		
		function result_object($query_result)
		{
			$r = mysql_fetch_object($query_result);
			if ($this->errno() != 0)
			{
				error('[mysql.inc.php] db->result_object(): Ergebnisabfrage schlug fehl.<br>MySQL Fehler: '.$this->error());
				return false;
			} else {
				return $r;
			}
		}
		function num_rows($query_result)
		{
			return mysql_num_rows($query_result);
		}

		          function getNewID($table,$id="id")
                {
                        $r=$this->query("select max($id)+1 as id from $table");
                        if($this->num_rows($r)>0)
                             {
                                    $row=$this->result($r);
                                   $r=$row[id];
                                    if($r!="" && $r!="NULL")
                                                return $r;
       }
			 return "1";
	}		
		function escape_string($s)
		{
			return mysql_escape_string($s);
		}

		/// check if string save for sql-processing
		function save_string($s)
		{
			$p=array("\"","'",";");
			foreach($p as $x)
			{
				if(strpos($s,$p))
					return "";
			}
			return $s;
				
		}	

		function update_entry($table,$id,$name,$entry)
		{
			$table=$this->save_string($table);
			$name=$this->save_string($name);
			if($table=="" || $name=="")
				return;
			$q=sprintf("update %s set %s='%s' where id='%i';",$table,$name,$this->escape_string($entry),$this->escape_string($id));
			$this->query($q);
		}
		
};

$db=new db();

function getsqlcol($q,$n)
{
	global $db;
	$r=$db->query($q);
	$res=array();
	for($i=0;$i<$db->num_rows($r);$i++)
	{
		$row=$db->result($r);
		$res[]=$row[$n];
	}
	return $res;
}

function getsql($q,$n)
{
	$c=getsqlcol($q,$n);
	if(sizeof($c)>0)
		return $c[0];
	return "";
}

function getsqlarray($q)
{
	global $db;
	$r=$db->query($q);
	$res=array();
	for($i=0;$i<$db->num_rows($r);$i++)
	{
		$row=$db->result($r);
		$res[$row[0]]=$row[1];
	}
	return $res;
}

}

?>