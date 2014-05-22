<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<title>Linux/fs/qnx4/bitmap.c - Linux Cross Reference - Free Electrons</title>
<link rel="stylesheet" href="/style.css" type="text/css" media="screen" />
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
<meta description="Linux Cross-Reference (LXR) service by Free Electrons. The easiest way to study Linux kernel sources. Available for all recent releases." />
<base href="http://lxr.free-electrons.com/"/>
</head>

<body>

<div id="wrapper">
<div id="wrapper2">
<div id="header">

	<div id="logo">
		<h1>Linux Cross Reference</h1>
		<h2><a href="http://free-electrons.com">Free Electrons</a></h2>
		<h2>Embedded Linux Experts</h2>
		<p>
		 &nbsp;&bull;&nbsp;<b><i>source navigation</i></b> &nbsp;&bull;&nbsp;<a href="diff/fs/qnx4/bitmap.c">diff markup</a> &nbsp;&bull;&nbsp;<a href="ident">identifier search</a> &nbsp;&bull;&nbsp;<a href="search">freetext search</a> &nbsp;&bull;&nbsp;
		</p>
	</div>
</div>

<div id="topbar">
  
  <p>Version: &nbsp;<a href="source/fs/qnx4/bitmap.c?v=2.6.34">2.6.34</a>&nbsp;<a href="source/fs/qnx4/bitmap.c?v=2.6.35">2.6.35</a>&nbsp;<a href="source/fs/qnx4/bitmap.c?v=2.6.36">2.6.36</a>&nbsp;<a href="source/fs/qnx4/bitmap.c?v=2.6.37">2.6.37</a>&nbsp;<a href="source/fs/qnx4/bitmap.c?v=2.6.38">2.6.38</a>&nbsp;<a href="source/fs/qnx4/bitmap.c?v=2.6.39">2.6.39</a>&nbsp;<a href="source/fs/qnx4/bitmap.c?v=3.0">3.0</a>&nbsp;<a href="source/fs/qnx4/bitmap.c?v=3.1">3.1</a>&nbsp;<a href="source/fs/qnx4/bitmap.c?v=3.2">3.2</a>&nbsp;<a href="source/fs/qnx4/bitmap.c?v=3.3">3.3</a>&nbsp;<a href="source/fs/qnx4/bitmap.c?v=3.4">3.4</a>&nbsp;<a href="source/fs/qnx4/bitmap.c?v=3.5">3.5</a>&nbsp;<a href="source/fs/qnx4/bitmap.c?v=3.6">3.6</a>&nbsp;<a href="source/fs/qnx4/bitmap.c?v=3.7">3.7</a>&nbsp;<a href="source/fs/qnx4/bitmap.c?v=3.8">3.8</a>&nbsp;<a href="source/fs/qnx4/bitmap.c?v=3.9">3.9</a>&nbsp;<a href="source/fs/qnx4/bitmap.c?v=3.10">3.10</a>&nbsp;<a href="source/fs/qnx4/bitmap.c?v=3.11">3.11</a>&nbsp;<a href="source/fs/qnx4/bitmap.c?v=3.12">3.12</a>&nbsp;<a href="source/fs/qnx4/bitmap.c?v=3.13">3.13</a>&nbsp;<b><i>3.14</i></b></p>
  
</div>

<h1><a href="source/">Linux</a>/<a href="source/fs/">fs</a>/<a href="source/fs/qnx4/">qnx4</a>/<a href="source/fs/qnx4/bitmap.c">bitmap.c</a></h1>
<div id="lxrcode"><pre>  <a name="L1" href="source/fs/qnx4/bitmap.c#L1">1</a> <b><i>/*</i></b>
  <a name="L2" href="source/fs/qnx4/bitmap.c#L2">2</a> <b><i> * QNX4 file system, Linux implementation.</i></b>
  <a name="L3" href="source/fs/qnx4/bitmap.c#L3">3</a> <b><i> *</i></b>
  <a name="L4" href="source/fs/qnx4/bitmap.c#L4">4</a> <b><i> * Version : 0.2.1</i></b>
  <a name="L5" href="source/fs/qnx4/bitmap.c#L5">5</a> <b><i> *</i></b>
  <a name="L6" href="source/fs/qnx4/bitmap.c#L6">6</a> <b><i> * Using parts of the xiafs filesystem.</i></b>
  <a name="L7" href="source/fs/qnx4/bitmap.c#L7">7</a> <b><i> *</i></b>
  <a name="L8" href="source/fs/qnx4/bitmap.c#L8">8</a> <b><i> * History :</i></b>
  <a name="L9" href="source/fs/qnx4/bitmap.c#L9">9</a> <b><i> *</i></b>
 <a name="L10" href="source/fs/qnx4/bitmap.c#L10">10</a> <b><i> * 28-05-1998 by Richard Frowijn : first release.</i></b>
 <a name="L11" href="source/fs/qnx4/bitmap.c#L11">11</a> <b><i> * 20-06-1998 by Frank Denis : basic optimisations.</i></b>
 <a name="L12" href="source/fs/qnx4/bitmap.c#L12">12</a> <b><i> * 25-06-1998 by Frank Denis : qnx4_is_free, qnx4_set_bitmap, qnx4_bmap .</i></b>
 <a name="L13" href="source/fs/qnx4/bitmap.c#L13">13</a> <b><i> * 28-06-1998 by Frank Denis : qnx4_free_inode (to be fixed) .</i></b>
 <a name="L14" href="source/fs/qnx4/bitmap.c#L14">14</a> <b><i> */</i></b>
 <a name="L15" href="source/fs/qnx4/bitmap.c#L15">15</a> 
 <a name="L16" href="source/fs/qnx4/bitmap.c#L16">16</a> #include &lt;linux/buffer_head.h&gt;
 <a name="L17" href="source/fs/qnx4/bitmap.c#L17">17</a> #include &lt;linux/bitops.h&gt;
 <a name="L18" href="source/fs/qnx4/bitmap.c#L18">18</a> #include "<a href="source/fs/qnx4/qnx4.h">qnx4.h</a>"
 <a name="L19" href="source/fs/qnx4/bitmap.c#L19">19</a> 
 <a name="L20" href="source/fs/qnx4/bitmap.c#L20">20</a> unsigned long <a href="ident?i=qnx4_count_free_blocks">qnx4_count_free_blocks</a>(struct <a href="ident?i=super_block">super_block</a> *<a href="ident?i=sb">sb</a>)
 <a name="L21" href="source/fs/qnx4/bitmap.c#L21">21</a> {
 <a name="L22" href="source/fs/qnx4/bitmap.c#L22">22</a>         int <a href="ident?i=start">start</a> = <a href="ident?i=le32_to_cpu">le32_to_cpu</a>(<a href="ident?i=qnx4_sb">qnx4_sb</a>(<a href="ident?i=sb">sb</a>)-&gt;BitMap-&gt;di_first_xtnt.xtnt_blk) - 1;
 <a name="L23" href="source/fs/qnx4/bitmap.c#L23">23</a>         int total = 0;
 <a name="L24" href="source/fs/qnx4/bitmap.c#L24">24</a>         int total_free = 0;
 <a name="L25" href="source/fs/qnx4/bitmap.c#L25">25</a>         int <a href="ident?i=offset">offset</a> = 0;
 <a name="L26" href="source/fs/qnx4/bitmap.c#L26">26</a>         int <a href="ident?i=size">size</a> = <a href="ident?i=le32_to_cpu">le32_to_cpu</a>(<a href="ident?i=qnx4_sb">qnx4_sb</a>(<a href="ident?i=sb">sb</a>)-&gt;BitMap-&gt;di_size);
 <a name="L27" href="source/fs/qnx4/bitmap.c#L27">27</a>         struct <a href="ident?i=buffer_head">buffer_head</a> *<a href="ident?i=bh">bh</a>;
 <a name="L28" href="source/fs/qnx4/bitmap.c#L28">28</a> 
 <a name="L29" href="source/fs/qnx4/bitmap.c#L29">29</a>         while (total &lt; <a href="ident?i=size">size</a>) {
 <a name="L30" href="source/fs/qnx4/bitmap.c#L30">30</a>                 int <a href="ident?i=bytes">bytes</a> = <a href="ident?i=min">min</a>(<a href="ident?i=size">size</a> - total, <a href="ident?i=QNX4_BLOCK_SIZE">QNX4_BLOCK_SIZE</a>);
 <a name="L31" href="source/fs/qnx4/bitmap.c#L31">31</a> 
 <a name="L32" href="source/fs/qnx4/bitmap.c#L32">32</a>                 if ((<a href="ident?i=bh">bh</a> = <a href="ident?i=sb_bread">sb_bread</a>(<a href="ident?i=sb">sb</a>, <a href="ident?i=start">start</a> + <a href="ident?i=offset">offset</a>)) == <a href="ident?i=NULL">NULL</a>) {
 <a name="L33" href="source/fs/qnx4/bitmap.c#L33">33</a>                         <a href="ident?i=printk">printk</a>(<a href="ident?i=KERN_ERR">KERN_ERR</a> <i>"qnx4: I/O error in counting free blocks\n"</i>);
 <a name="L34" href="source/fs/qnx4/bitmap.c#L34">34</a>                         break;
 <a name="L35" href="source/fs/qnx4/bitmap.c#L35">35</a>                 }
 <a name="L36" href="source/fs/qnx4/bitmap.c#L36">36</a>                 total_free += <a href="ident?i=bytes">bytes</a> * <a href="ident?i=BITS_PER_BYTE">BITS_PER_BYTE</a> -
 <a name="L37" href="source/fs/qnx4/bitmap.c#L37">37</a>                                 <a href="ident?i=memweight">memweight</a>(<a href="ident?i=bh">bh</a>-&gt;b_data, <a href="ident?i=bytes">bytes</a>);
 <a name="L38" href="source/fs/qnx4/bitmap.c#L38">38</a>                 <a href="ident?i=brelse">brelse</a>(<a href="ident?i=bh">bh</a>);
 <a name="L39" href="source/fs/qnx4/bitmap.c#L39">39</a>                 total += <a href="ident?i=bytes">bytes</a>;
 <a name="L40" href="source/fs/qnx4/bitmap.c#L40">40</a>                 <a href="ident?i=offset">offset</a>++;
 <a name="L41" href="source/fs/qnx4/bitmap.c#L41">41</a>         }
 <a name="L42" href="source/fs/qnx4/bitmap.c#L42">42</a> 
 <a name="L43" href="source/fs/qnx4/bitmap.c#L43">43</a>         return total_free;
 <a name="L44" href="source/fs/qnx4/bitmap.c#L44">44</a> }
 <a name="L45" href="source/fs/qnx4/bitmap.c#L45">45</a> </pre></div><p>
This page was automatically generated by <a href="http://lxr.linux.no">LXR</a> 0.3.1 (<a href="http://free-electrons.com/pub/source/lxr-0.3.1-fe1.tar.bz2">source</a>).
&nbsp;&bull;&nbsp;
Linux is a registered trademark of Linus Torvalds
&nbsp;&bull;&nbsp;
<a href="http://free-electrons.com/company/contact/">Contact us</a>
</p>

<div id="menu">
<ul>
   <li><a href="http://free-electrons.com/">Home</a></li>
   <li><a href="http://free-electrons.com/development/" title="Development">Development</a></li>
   <li><a href="http://free-electrons.com/services/" title="Services">Services</a></li>
   <li><a href="http://free-electrons.com/training/" title="Training">Training</a></li>
   <li><a href="http://free-electrons.com/docs/" title="Docs">Docs</a></li>
   <li><a href="http://free-electrons.com/community/" title="Community">Community</a></li>
   <li><a href="http://free-electrons.com/company/" title="Company">Company</a></li>
   <li><a href="http://free-electrons.com/blog/" title="Blog">Blog</a></li>
</ul>
</div>

</div>
</div>
</body>
</html>
