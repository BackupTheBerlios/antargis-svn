<?

// Model for news

class NewsItem
{
	var $id="";
	var $date="";
	var $title="";
	var $text="";

	function NewsItem($id)
	{
		global $db;
		$row=$db->getRowById("main_news",$id);
		$this->id=$row[id];
		$this->date=$row[date];
		$this->title=$row[title];
		$this->text=$row[text];
	}

	function save()
	{
		global $db;
		$db->update_entry("main_news",$this->id,"date",$this->date);
		$db->update_entry("main_news",$this->id,"title",$this->title);
		$db->update_entry("main_news",$this->id,"text",$this->text);
	}
}

function getNewsIds()
{
	global $db;
	return $db->getIds("main_news",array("date"));
}

?>