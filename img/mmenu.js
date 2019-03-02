root=''

rnode = AddRootNode('Начало',root+'/index.html');

rnode = AddRootNode('Про&nbsp;электронику',root+'/micros/');
	node1 = AddNode(rnode, 'Проекты&nbsp;на&nbsp;STM32',root+'/micros/stm32/');
		AddNode(node1, 'Часы',root+'/micros/stm32/7-seg-clock/');
		AddNode(node1, 'Часы&nbsp;с&nbsp;FM-радио',root+'/micros/stm32/led-matrix-clock-fm/');
	node1 = AddNode(rnode, 'Проекты&nbsp;на&nbsp;AT89C2051',root+'/micros/at89c2051/');
		AddNode(node1, 'Светофор',root+'/micros/at89c2051/svetofor/');
		AddNode(node1, 'Тахометр',root+'/micros/at89c2051/tachometer/');
		AddNode(node1, 'Регулятор&nbsp;мощности',root+'/micros/at89c2051/power-reg/');
	node1 = AddNode(rnode, 'Проекты&nbsp;на&nbsp;AtMega',root+'/micros/mega/');
		AddNode(node1, 'Зарядное&nbsp;устройство',root+'/micros/mega/charger/');
		AddNode(node1, 'Дисплей от Nokia&nbsp;6100',root+'/micros/lcd/nokia6100/');
	node1 = AddNode(rnode, 'Проекты&nbsp;на&nbsp;AtTiny',root+'/micros/tiny/');
		AddNode(node1, 'Семисегментный&nbsp;индикатор',root+'/micros/tiny/leds/');
		AddNode(node1, 'Дисплей от Nokia&nbsp;3310',root+'/micros/lcd/nokia3310/');
	node1 = AddNode(rnode, 'Другие&nbsp;проекты',root+'/micros/other/');
		AddNode(node1, 'Нечто&nbsp;лучше&nbsp;КРЕНки',root+'/micros/other/step-down-34063/');
		AddNode(node1, 'Светодиодная&nbsp;матрица',root+'/micros/other/led-matrix-max7219/');
	node1 = AddNode(rnode, 'ЖК&nbsp;дисплеи',root+'/micros/lcd/');
		AddNode(node1, 'Nokia&nbsp;3310',root+'/micros/lcd/nokia3310/');
		AddNode(node1, 'Nokia&nbsp;7110',root+'/micros/lcd/nokia7110/');
		AddNode(node1, 'Nokia&nbsp;6100',root+'/micros/lcd/nokia6100/');

rnode = AddRootNode('Про&nbsp;роботов',root+'/robots/');
	node1 = AddNode(rnode, 'Драйвер&nbsp;мотора',root+'/robots/motor-driver/');
//	node1 = AddNode(rnode, 'Радиолинк',root+'/robots/radio-link/');
//	node1 = AddNode(rnode, 'Сервы',root+'/robots/servo-controller/');
	node1 = AddNode(rnode, 'Колёсная&nbsp;база',root+'/robots/wheel-base/');

rnode = AddRootNode('Про&nbsp;светодиоды',root+'/leds/');
	node1 = AddNode(rnode, 'Светодиодная&nbsp;люстра&nbsp;на&nbsp;21Вт',root+'/leds/led-lamp-21w/');
	node1 = AddNode(rnode, 'Драйвер&nbsp;на&nbsp;300мА',root+'/leds/led-driver-300ma/');

rnode = AddRootNode('Про&nbsp;компьютеры',root+'/computers/');
	node1 = AddNode(rnode, 'Самодельный&nbsp;NAS',root+'/computers/nas/');

//rnode = AddRootNode('Прочее',root+'/other/');
//rnode = AddRootNode('Контакты',root+'/contacts.html');
//rnode = AddRootNode('Карта сайта',root+'/sitemap.html');

function ToggleTree()
{
	treediv = window.document.getElementById('nodes');
	contentdiv = window.document.getElementById('content');
	if (treediv)
	{
		if (treediv.style.display == 'none')
		{
			treediv.style.display = '';
			contentdiv.style.marginLeft = treediv.style.width;
		}
		else
		{
			treediv.style.display = 'none';
			contentdiv.style.marginLeft = 108;
		}
	}
}

function SiteHead() {

	document.title='AHTOXA.NET : ' + GetTitlePath();
	document.write('<div id="wspacer">');
	document.write('<a name="top"></a>');
	document.write('<div id="document">');
	document.write('<div id="logo"><a href="'+root+'/">&nbsp;</a></div>');
	document.write('<div id="siteTitle"><a href="'+root+'/">AHTOXA.NET</a></div>');
	document.write('<div id="doctree">');
	document.write('<a  class="treelink" href="javascript:void(0);" onClick=ToggleTree() title="Спрятать/показать оглавление">Оглавление</a>');
	document.write('<div id="nodes">');

	DisplayNodes();

	document.write('</div></div>'); // nodes, doctree

	document.write('<hr class="invisible" />');
	document.write('<div id="content">');
	DisplayLinkedPath();

}

function SiteFoot() {
	document.write('</div></div>'); //  content, document
	document.write('<div id="bottom">');
	document.write('<a href=#top>top</a>');
	document.write('<span class="copyright">&copy; AHTOXA, 2005-2015.</span><br/><br/>');
	document.write('<div id="bleft"><a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/deed.ru"><img alt="Лицензия Creative Commons" style="border-width:0" src="/img/CC-BY-SA-88x31.png" /></a></div>');
	document.write('<div id="bcenter">Если не указано иное, содержимое данного сайта доступно на условиях <a rel="license" href="http://creativecommons.org/licenses/by-sa/4.0/deed.ru">лицензии Creative Commons «Attribution-ShareAlike» 4.0 Всемирная</a><br /><br /></div>');
	document.write('</div></div>'); // bottom, wspacer
}

