var nodes = new Array();
	ipath = '/img/st/';
	isize = 18;
    nodescount = 0;
    minus1 = new Image();
    minus1.src=ipath+'m1l.gif';
    minus2 = new Image();
    minus2.src=ipath+'m2l.gif';
    plus1 = new Image();
    plus1.src=ipath+'p1l.gif';
    plus2 = new Image();
    plus2.src=ipath+'p2l.gif';
    folderopen = new Image();
    folderopen.src=ipath+'f.gif';
    folderclosed = new Image();
    folderclosed.src=ipath+'fo.gif';
    folderopen_checked = new Image();
    folderopen_checked.src=ipath+'fc.gif';
    folderclosed_checked = new Image();
    folderclosed_checked.src=ipath+'foc.gif';
    ExpandText='Expand';
    CollapseText='Collapse';

function CreateNode(parentid, txt, lnk) {
    this.id=nodescount++;
    this.parentid=parentid;
    this.txt=txt;
    this.lnk=lnk;
    this.parent=null;
    this.expanded=false;
    this.image='im'+this.id;
    this.image1='ims'+this.id;
    this.childarea='ca'+this.id;
    this.last=false;
    this.current=false;
    this.children = new Array();
}

function AddChild(n1,n2) {
var j;
    n1.children[n1.children.length]=n2;
    n2.last=true;
    n2.parent=n1;
    for (j=0; j<n1.children.length-1; j++) n1.children[j].last=false; 
}

function CatchChildren(node){
var i, n1;
    for (i = 0; i < nodes.length; i++) {
        n1 = nodes[i];
        if (n1.parentid == node.id) AddChild(node,n1)
	}
}

function WriteImg(im,id,alt) {
	document.write('<img border="0" width="'+isize+'" height="'+isize+'" ');
	if (alt) document.write('title="'+alt+'" ');
	else document.write('title="" ');
	document.write('src="'+ipath+im+'" ');
    if (id) document.write('id="'+id+'"');
	document.write('>');
}

function WriteImg1(im,id)
{
	var s='<img border="0" width="'+isize+'" height="'+isize+'" src="'+ipath;
    if (id)	s=s+im+'" id="'+id+'">';
    else	s=s+im+'">';
    return s;
}

function ident(node)
{
	var	n2, st='';

	n2=node.parent;
	while (n2)
	{
		if (n2.last)	st=WriteImg1('e.gif','')+st;
		else			st=WriteImg1('l.gif','')+st;
		n2=n2.parent; 
	}
	document.write(st);
}

function DisplayNode(node) {
var i,j,k, n1;
	document.write('<table class="treetable" border=0 cellpadding=0 cellspacing=0><tr><td valign=middle>');
	ident(node);    
	if (node.children.length)
	{
		document.write('<a onClick=ToggleNode('+node.id+')>');
		if (node.last)  
			WriteImg('p1l.gif', node.image, ExpandText);
		else
			WriteImg('p2l.gif', node.image, ExpandText);
		if (node.current)
			WriteImg('fc.gif',node.image1);
		else
			WriteImg('f.gif',node.image1);
		document.write('</a></td><td valign=middle>&nbsp;');
		if (node.lnk) document.write('<a class="treelink" href="'+node.lnk+'">');
		document.write(node.txt);
		if (node.lnk) document.write('</a>');
		document.write('</td></tr></table>');
		document.write('<div id="'+node.childarea+'" style="display:none">');
        for (k = 0; k < node.children.length; k++)
		{
            n1 = node.children[k];
            DisplayNode(n1);
		}
		document.write('</div>');
	}
	else
	{
		if ( node.last )	WriteImg('l1.gif',node.image);
		else				WriteImg('l2.gif',node.image);

		document.write('<a href="'+node.lnk+'">');
		if (node.current)
			WriteImg('sc.gif',node.image1);
		else
			WriteImg('s.gif',node.image1);
		document.write('</a></td><td valign=middle>&nbsp;');
		document.write('<a class="treelink" href="'+node.lnk+'">');
		document.write('&nbsp;'+node.txt);
		document.write('</a></td></tr></table>');
	}
}

function SaveState(node) {
	if (node.lnk) return;
	document.cookie=cookieName+"="+escape(cookieContent)+"; path="+escape('/')+"";
}

function ToggleNode(nodenum) {
	node = nodes[nodenum];
	div = window.document.getElementById(node.childarea);
	gif = window.document.getElementById(node.image);
	gif1 = window.document.getElementById(node.image1);
	if (div) {
		if (node.expanded) {
			node.expanded=false;
			gif.title = ExpandText;
			if (node.last)
				gif.src=plus1.src;
			else
				gif.src=plus2.src;
			if (node.current)
				gif1.src=folderopen_checked.src;
			else
				gif1.src=folderopen.src;
			div.style.display = 'none';
		} else {
			node.expanded=true;
			gif.title=CollapseText;
			if (node.last) 
				gif.src=minus1.src;
			else
				gif.src=minus2.src;
			if (node.current)
				gif1.src=folderclosed_checked.src;
			else
				gif1.src=folderclosed.src;
			div.style.display = '';
		}
		gif1.title=gif.title;
	}
}

function FindCurrentNode() {

	var path = window.location.pathname;

    for (i = 0; i < nodes.length; i++) {
        node = nodes[i];
		if (node.lnk == path) return node;
    }

	for (i = 0; i < nodes.length; i++) {
        node = nodes[i];
		if (node.lnk + 'index.html' == path) return node;
	}

	for (i = 0; i < nodes.length; i++) {
        node = nodes[i];
		if (node.lnk + '/index.html' == path) return node;
	}

	for (i = 0; i < nodes.length; i++) {
        node = nodes[i];
		if (node.lnk == path+ 'index.html' ) return node;
	}

	for (i = 0; i < nodes.length; i++) {
        node = nodes[i];
		if (node.lnk == path+ '/index.html' ) return node;
	}

	return 0;
}

function DisplayNodes() {

	
	cnode = FindCurrentNode();
	if (cnode)
	    cnode.current=true;

    for (i = 0; i < nodes.length; i++) {
        node = nodes[i];
        CatchChildren(node);
    }

    for (i = 0; i < nodes.length; i++) {
        node=nodes[i];
        if (node.parentid==-1) {
            node.last=true;
            for (j=i+1; j<nodes.length; j++) {
                if (nodes[j].parentid==-1) {
                    node.last=false;
                    break;
                }
            }
            DisplayNode(node);
    	}
	}

	cnode = FindCurrentNode();
	if (cnode) {
		if (cnode.children.length)
			if (!cnode.expanded)
				ToggleNode(cnode.id);
		while (cnode.parent) {
			cnode = cnode.parent;
			if (!cnode.expanded)
				ToggleNode(cnode.id);
		}
	}
}

function DisplayNodeUl(node) 
{

	var i;

	document.write('<li>');

	if (node.lnk) document.write('<a href="'+node.lnk+'">');
	document.write(node.txt);
	if (node.lnk) document.write('</a>');

	if (node.children.length) 
	{
		document.write('<ul>');
        for (i = 0; i < node.children.length; i++)
            DisplayNodeUl(node.children[i]);
		document.write('</ul>');
	}
}

function DisplaySubNodes()
{
	var i;
	cnode = FindCurrentNode();

	if (cnode) 
	{
		document.write('<p><ul>');
        for (i = 0; i < cnode.children.length; i++)
            DisplayNodeUl(cnode.children[i]);
		document.write('</ul>');
	}
}

function GetPath()
{
	var s = '';
	cnode = FindCurrentNode();
	if (cnode) 
	{
		s = cnode.txt;
		while (cnode.parent) {
			cnode = cnode.parent;
			s = cnode.txt + '&nbsp;&raquo;&nbsp;'+s;
		}
	}
	return s;
}

function GetTitlePath()
{
	var s = '';
	cnode = FindCurrentNode();
	if (cnode) 
		s = cnode.txt.replace(/\&nbsp\;/g,' ');
	return s;
}

function DisplayPath()
{
	document.write(GetPath());
}

function GetLinkedPath()
{
	var s = '';
	cnode = FindCurrentNode();
	if (cnode) 
	{
		s = cnode.txt;
		while (cnode.parent) {
			cnode = cnode.parent;
			s = '<a href="'+cnode.lnk+'">'+cnode.txt +'</a>&nbsp;&raquo;&nbsp;'+s;
		}
	}
	return s;
}

function DisplayLinkedPath()
{
	document.write(GetLinkedPath());
}

function AddNode(parent, txt, lnk)
{
	node = new CreateNode(parent.id, txt, lnk);
	node.parent = parent;
	nodes[node.id]=node;
	return node;
}

function AddRootNode(txt, lnk)
{
	node = new CreateNode(-1, txt, lnk);
	nodes[node.id]=node;
	return node;
}
