<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<title>Linux/fs/qnx4/qnx4.h - Linux Cross Reference - Free Electrons</title>
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
		 &nbsp;&bull;&nbsp;<b><i>source navigation</i></b> &nbsp;&bull;&nbsp;<a href="diff/fs/qnx4/qnx4.h">diff markup</a> &nbsp;&bull;&nbsp;<a href="ident">identifier search</a> &nbsp;&bull;&nbsp;<a href="search">freetext search</a> &nbsp;&bull;&nbsp;
		</p>
	</div>
</div>

<div id="topbar">
  
  <p>Version: &nbsp;<a href="source/fs/qnx4/qnx4.h?v=2.6.34">2.6.34</a>&nbsp;<a href="source/fs/qnx4/qnx4.h?v=2.6.35">2.6.35</a>&nbsp;<a href="source/fs/qnx4/qnx4.h?v=2.6.36">2.6.36</a>&nbsp;<a href="source/fs/qnx4/qnx4.h?v=2.6.37">2.6.37</a>&nbsp;<a href="source/fs/qnx4/qnx4.h?v=2.6.38">2.6.38</a>&nbsp;<a href="source/fs/qnx4/qnx4.h?v=2.6.39">2.6.39</a>&nbsp;<a href="source/fs/qnx4/qnx4.h?v=3.0">3.0</a>&nbsp;<a href="source/fs/qnx4/qnx4.h?v=3.1">3.1</a>&nbsp;<a href="source/fs/qnx4/qnx4.h?v=3.2">3.2</a>&nbsp;<a href="source/fs/qnx4/qnx4.h?v=3.3">3.3</a>&nbsp;<a href="source/fs/qnx4/qnx4.h?v=3.4">3.4</a>&nbsp;<a href="source/fs/qnx4/qnx4.h?v=3.5">3.5</a>&nbsp;<a href="source/fs/qnx4/qnx4.h?v=3.6">3.6</a>&nbsp;<a href="source/fs/qnx4/qnx4.h?v=3.7">3.7</a>&nbsp;<a href="source/fs/qnx4/qnx4.h?v=3.8">3.8</a>&nbsp;<a href="source/fs/qnx4/qnx4.h?v=3.9">3.9</a>&nbsp;<a href="source/fs/qnx4/qnx4.h?v=3.10">3.10</a>&nbsp;<a href="source/fs/qnx4/qnx4.h?v=3.11">3.11</a>&nbsp;<a href="source/fs/qnx4/qnx4.h?v=3.12">3.12</a>&nbsp;<a href="source/fs/qnx4/qnx4.h?v=3.13">3.13</a>&nbsp;<b><i>3.14</i></b></p>
  
</div>

<h1><a href="source/">Linux</a>/<a href="source/fs/">fs</a>/<a href="source/fs/qnx4/">qnx4</a>/<a href="source/fs/qnx4/qnx4.h">qnx4.h</a></h1>
<div id="lxrcode"><pre>  <a name="L1" href="source/fs/qnx4/qnx4.h#L1">1</a> #include &lt;linux/fs.h&gt;
  <a name="L2" href="source/fs/qnx4/qnx4.h#L2">2</a> #include &lt;linux/qnx4_fs.h&gt;
  <a name="L3" href="source/fs/qnx4/qnx4.h#L3">3</a> 
  <a name="L4" href="source/fs/qnx4/qnx4.h#L4">4</a> #define <a href="ident?i=QNX4_DEBUG">QNX4_DEBUG</a> 0
  <a name="L5" href="source/fs/qnx4/qnx4.h#L5">5</a> 
  <a name="L6" href="source/fs/qnx4/qnx4.h#L6">6</a> #if <a href="ident?i=QNX4_DEBUG">QNX4_DEBUG</a>
  <a name="L7" href="source/fs/qnx4/qnx4.h#L7">7</a> #define <a href="ident?i=QNX4DEBUG">QNX4DEBUG</a>(<a href="ident?i=X">X</a>) <a href="ident?i=printk">printk</a> <a href="ident?i=X">X</a>
  <a name="L8" href="source/fs/qnx4/qnx4.h#L8">8</a> #else
  <a name="L9" href="source/fs/qnx4/qnx4.h#L9">9</a> #define <a href="ident?i=QNX4DEBUG">QNX4DEBUG</a>(<a href="ident?i=X">X</a>) (void) 0
 <a name="L10" href="source/fs/qnx4/qnx4.h#L10">10</a> #endif
 <a name="L11" href="source/fs/qnx4/qnx4.h#L11">11</a> 
 <a name="L12" href="source/fs/qnx4/qnx4.h#L12">12</a> struct <a href="ident?i=qnx4_sb_info">qnx4_sb_info</a> {
 <a name="L13" href="source/fs/qnx4/qnx4.h#L13">13</a>         unsigned int            Version;        <b><i>/* may be useful */</i></b>
 <a name="L14" href="source/fs/qnx4/qnx4.h#L14">14</a>         struct <a href="ident?i=qnx4_inode_entry">qnx4_inode_entry</a> *BitMap;        <b><i>/* useful */</i></b>
 <a name="L15" href="source/fs/qnx4/qnx4.h#L15">15</a> };
 <a name="L16" href="source/fs/qnx4/qnx4.h#L16">16</a> 
 <a name="L17" href="source/fs/qnx4/qnx4.h#L17">17</a> struct <a href="ident?i=qnx4_inode_info">qnx4_inode_info</a> {
 <a name="L18" href="source/fs/qnx4/qnx4.h#L18">18</a>         struct <a href="ident?i=qnx4_inode_entry">qnx4_inode_entry</a> <a href="ident?i=raw">raw</a>;
 <a name="L19" href="source/fs/qnx4/qnx4.h#L19">19</a>         <a href="ident?i=loff_t">loff_t</a> mmu_private;
 <a name="L20" href="source/fs/qnx4/qnx4.h#L20">20</a>         struct <a href="ident?i=inode">inode</a> vfs_inode;
 <a name="L21" href="source/fs/qnx4/qnx4.h#L21">21</a> };
 <a name="L22" href="source/fs/qnx4/qnx4.h#L22">22</a> 
 <a name="L23" href="source/fs/qnx4/qnx4.h#L23">23</a> extern struct <a href="ident?i=inode">inode</a> *<a href="ident?i=qnx4_iget">qnx4_iget</a>(struct <a href="ident?i=super_block">super_block</a> *, unsigned long);
 <a name="L24" href="source/fs/qnx4/qnx4.h#L24">24</a> extern struct <a href="ident?i=dentry">dentry</a> *<a href="ident?i=qnx4_lookup">qnx4_lookup</a>(struct <a href="ident?i=inode">inode</a> *<a href="ident?i=dir">dir</a>, struct <a href="ident?i=dentry">dentry</a> *<a href="ident?i=dentry">dentry</a>, unsigned int <a href="ident?i=flags">flags</a>);
 <a name="L25" href="source/fs/qnx4/qnx4.h#L25">25</a> extern unsigned long <a href="ident?i=qnx4_count_free_blocks">qnx4_count_free_blocks</a>(struct <a href="ident?i=super_block">super_block</a> *<a href="ident?i=sb">sb</a>);
 <a name="L26" href="source/fs/qnx4/qnx4.h#L26">26</a> extern unsigned long <a href="ident?i=qnx4_block_map">qnx4_block_map</a>(struct <a href="ident?i=inode">inode</a> *<a href="ident?i=inode">inode</a>, long iblock);
 <a name="L27" href="source/fs/qnx4/qnx4.h#L27">27</a> 
 <a name="L28" href="source/fs/qnx4/qnx4.h#L28">28</a> extern const struct <a href="ident?i=inode_operations">inode_operations</a> <a href="ident?i=qnx4_dir_inode_operations">qnx4_dir_inode_operations</a>;
 <a name="L29" href="source/fs/qnx4/qnx4.h#L29">29</a> extern const struct <a href="ident?i=file_operations">file_operations</a> <a href="ident?i=qnx4_dir_operations">qnx4_dir_operations</a>;
 <a name="L30" href="source/fs/qnx4/qnx4.h#L30">30</a> extern int <a href="ident?i=qnx4_is_free">qnx4_is_free</a>(struct <a href="ident?i=super_block">super_block</a> *<a href="ident?i=sb">sb</a>, long <a href="ident?i=block">block</a>);
 <a name="L31" href="source/fs/qnx4/qnx4.h#L31">31</a> 
 <a name="L32" href="source/fs/qnx4/qnx4.h#L32">32</a> static inline struct <a href="ident?i=qnx4_sb_info">qnx4_sb_info</a> *<a href="ident?i=qnx4_sb">qnx4_sb</a>(struct <a href="ident?i=super_block">super_block</a> *<a href="ident?i=sb">sb</a>)
 <a name="L33" href="source/fs/qnx4/qnx4.h#L33">33</a> {
 <a name="L34" href="source/fs/qnx4/qnx4.h#L34">34</a>         return <a href="ident?i=sb">sb</a>-&gt;s_fs_info;
 <a name="L35" href="source/fs/qnx4/qnx4.h#L35">35</a> }
 <a name="L36" href="source/fs/qnx4/qnx4.h#L36">36</a> 
 <a name="L37" href="source/fs/qnx4/qnx4.h#L37">37</a> static inline struct <a href="ident?i=qnx4_inode_info">qnx4_inode_info</a> *<a href="ident?i=qnx4_i">qnx4_i</a>(struct <a href="ident?i=inode">inode</a> *<a href="ident?i=inode">inode</a>)
 <a name="L38" href="source/fs/qnx4/qnx4.h#L38">38</a> {
 <a name="L39" href="source/fs/qnx4/qnx4.h#L39">39</a>         return <a href="ident?i=container_of">container_of</a>(<a href="ident?i=inode">inode</a>, struct <a href="ident?i=qnx4_inode_info">qnx4_inode_info</a>, vfs_inode);
 <a name="L40" href="source/fs/qnx4/qnx4.h#L40">40</a> }
 <a name="L41" href="source/fs/qnx4/qnx4.h#L41">41</a> 
 <a name="L42" href="source/fs/qnx4/qnx4.h#L42">42</a> static inline struct <a href="ident?i=qnx4_inode_entry">qnx4_inode_entry</a> *<a href="ident?i=qnx4_raw_inode">qnx4_raw_inode</a>(struct <a href="ident?i=inode">inode</a> *<a href="ident?i=inode">inode</a>)
 <a name="L43" href="source/fs/qnx4/qnx4.h#L43">43</a> {
 <a name="L44" href="source/fs/qnx4/qnx4.h#L44">44</a>         return &amp;<a href="ident?i=qnx4_i">qnx4_i</a>(<a href="ident?i=inode">inode</a>)-&gt;<a href="ident?i=raw">raw</a>;
 <a name="L45" href="source/fs/qnx4/qnx4.h#L45">45</a> }
 <a name="L46" href="source/fs/qnx4/qnx4.h#L46">46</a> </pre></div><p>
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
