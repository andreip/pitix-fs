<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<title>Linux/fs/qnx4/dir.c - Linux Cross Reference - Free Electrons</title>
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
		 &nbsp;&bull;&nbsp;<b><i>source navigation</i></b> &nbsp;&bull;&nbsp;<a href="diff/fs/qnx4/dir.c">diff markup</a> &nbsp;&bull;&nbsp;<a href="ident">identifier search</a> &nbsp;&bull;&nbsp;<a href="search">freetext search</a> &nbsp;&bull;&nbsp;
		</p>
	</div>
</div>

<div id="topbar">
  
  <p>Version: &nbsp;<a href="source/fs/qnx4/dir.c?v=2.6.34">2.6.34</a>&nbsp;<a href="source/fs/qnx4/dir.c?v=2.6.35">2.6.35</a>&nbsp;<a href="source/fs/qnx4/dir.c?v=2.6.36">2.6.36</a>&nbsp;<a href="source/fs/qnx4/dir.c?v=2.6.37">2.6.37</a>&nbsp;<a href="source/fs/qnx4/dir.c?v=2.6.38">2.6.38</a>&nbsp;<a href="source/fs/qnx4/dir.c?v=2.6.39">2.6.39</a>&nbsp;<a href="source/fs/qnx4/dir.c?v=3.0">3.0</a>&nbsp;<a href="source/fs/qnx4/dir.c?v=3.1">3.1</a>&nbsp;<a href="source/fs/qnx4/dir.c?v=3.2">3.2</a>&nbsp;<a href="source/fs/qnx4/dir.c?v=3.3">3.3</a>&nbsp;<a href="source/fs/qnx4/dir.c?v=3.4">3.4</a>&nbsp;<a href="source/fs/qnx4/dir.c?v=3.5">3.5</a>&nbsp;<a href="source/fs/qnx4/dir.c?v=3.6">3.6</a>&nbsp;<a href="source/fs/qnx4/dir.c?v=3.7">3.7</a>&nbsp;<a href="source/fs/qnx4/dir.c?v=3.8">3.8</a>&nbsp;<a href="source/fs/qnx4/dir.c?v=3.9">3.9</a>&nbsp;<a href="source/fs/qnx4/dir.c?v=3.10">3.10</a>&nbsp;<a href="source/fs/qnx4/dir.c?v=3.11">3.11</a>&nbsp;<a href="source/fs/qnx4/dir.c?v=3.12">3.12</a>&nbsp;<a href="source/fs/qnx4/dir.c?v=3.13">3.13</a>&nbsp;<b><i>3.14</i></b></p>
  
</div>

<h1><a href="source/">Linux</a>/<a href="source/fs/">fs</a>/<a href="source/fs/qnx4/">qnx4</a>/<a href="source/fs/qnx4/dir.c">dir.c</a></h1>
<div id="lxrcode"><pre>  <a name="L1" href="source/fs/qnx4/dir.c#L1">1</a> <b><i>/*</i></b>
  <a name="L2" href="source/fs/qnx4/dir.c#L2">2</a> <b><i> * QNX4 file system, Linux implementation.</i></b>
  <a name="L3" href="source/fs/qnx4/dir.c#L3">3</a> <b><i> *</i></b>
  <a name="L4" href="source/fs/qnx4/dir.c#L4">4</a> <b><i> * Version : 0.2.1</i></b>
  <a name="L5" href="source/fs/qnx4/dir.c#L5">5</a> <b><i> *</i></b>
  <a name="L6" href="source/fs/qnx4/dir.c#L6">6</a> <b><i> * Using parts of the xiafs filesystem.</i></b>
  <a name="L7" href="source/fs/qnx4/dir.c#L7">7</a> <b><i> *</i></b>
  <a name="L8" href="source/fs/qnx4/dir.c#L8">8</a> <b><i> * History :</i></b>
  <a name="L9" href="source/fs/qnx4/dir.c#L9">9</a> <b><i> *</i></b>
 <a name="L10" href="source/fs/qnx4/dir.c#L10">10</a> <b><i> * 28-05-1998 by Richard Frowijn : first release.</i></b>
 <a name="L11" href="source/fs/qnx4/dir.c#L11">11</a> <b><i> * 20-06-1998 by Frank Denis : Linux 2.1.99+ &amp; dcache support.</i></b>
 <a name="L12" href="source/fs/qnx4/dir.c#L12">12</a> <b><i> */</i></b>
 <a name="L13" href="source/fs/qnx4/dir.c#L13">13</a> 
 <a name="L14" href="source/fs/qnx4/dir.c#L14">14</a> #include &lt;linux/buffer_head.h&gt;
 <a name="L15" href="source/fs/qnx4/dir.c#L15">15</a> #include "<a href="source/fs/qnx4/qnx4.h">qnx4.h</a>"
 <a name="L16" href="source/fs/qnx4/dir.c#L16">16</a> 
 <a name="L17" href="source/fs/qnx4/dir.c#L17">17</a> static int <a href="ident?i=qnx4_readdir">qnx4_readdir</a>(struct <a href="ident?i=file">file</a> *<a href="ident?i=file">file</a>, struct <a href="ident?i=dir_context">dir_context</a> *<a href="ident?i=ctx">ctx</a>)
 <a name="L18" href="source/fs/qnx4/dir.c#L18">18</a> {
 <a name="L19" href="source/fs/qnx4/dir.c#L19">19</a>         struct <a href="ident?i=inode">inode</a> *<a href="ident?i=inode">inode</a> = <a href="ident?i=file_inode">file_inode</a>(<a href="ident?i=file">file</a>);
 <a name="L20" href="source/fs/qnx4/dir.c#L20">20</a>         unsigned int <a href="ident?i=offset">offset</a>;
 <a name="L21" href="source/fs/qnx4/dir.c#L21">21</a>         struct <a href="ident?i=buffer_head">buffer_head</a> *<a href="ident?i=bh">bh</a>;
 <a name="L22" href="source/fs/qnx4/dir.c#L22">22</a>         struct <a href="ident?i=qnx4_inode_entry">qnx4_inode_entry</a> *<a href="ident?i=de">de</a>;
 <a name="L23" href="source/fs/qnx4/dir.c#L23">23</a>         struct <a href="ident?i=qnx4_link_info">qnx4_link_info</a> *le;
 <a name="L24" href="source/fs/qnx4/dir.c#L24">24</a>         unsigned long blknum;
 <a name="L25" href="source/fs/qnx4/dir.c#L25">25</a>         int ix, <a href="ident?i=ino">ino</a>;
 <a name="L26" href="source/fs/qnx4/dir.c#L26">26</a>         int <a href="ident?i=size">size</a>;
 <a name="L27" href="source/fs/qnx4/dir.c#L27">27</a> 
 <a name="L28" href="source/fs/qnx4/dir.c#L28">28</a>         <a href="ident?i=QNX4DEBUG">QNX4DEBUG</a>((<a href="ident?i=KERN_INFO">KERN_INFO</a> <i>"qnx4_readdir:i_size = %ld\n"</i>, (long) <a href="ident?i=inode">inode</a>-&gt;i_size));
 <a name="L29" href="source/fs/qnx4/dir.c#L29">29</a>         <a href="ident?i=QNX4DEBUG">QNX4DEBUG</a>((<a href="ident?i=KERN_INFO">KERN_INFO</a> <i>"pos                 = %ld\n"</i>, (long) <a href="ident?i=ctx">ctx</a>-&gt;<a href="ident?i=pos">pos</a>));
 <a name="L30" href="source/fs/qnx4/dir.c#L30">30</a> 
 <a name="L31" href="source/fs/qnx4/dir.c#L31">31</a>         while (<a href="ident?i=ctx">ctx</a>-&gt;<a href="ident?i=pos">pos</a> &lt; <a href="ident?i=inode">inode</a>-&gt;i_size) {
 <a name="L32" href="source/fs/qnx4/dir.c#L32">32</a>                 blknum = <a href="ident?i=qnx4_block_map">qnx4_block_map</a>(<a href="ident?i=inode">inode</a>, <a href="ident?i=ctx">ctx</a>-&gt;<a href="ident?i=pos">pos</a> &gt;&gt; <a href="ident?i=QNX4_BLOCK_SIZE_BITS">QNX4_BLOCK_SIZE_BITS</a>);
 <a name="L33" href="source/fs/qnx4/dir.c#L33">33</a>                 <a href="ident?i=bh">bh</a> = <a href="ident?i=sb_bread">sb_bread</a>(<a href="ident?i=inode">inode</a>-&gt;i_sb, blknum);
 <a name="L34" href="source/fs/qnx4/dir.c#L34">34</a>                 if (<a href="ident?i=bh">bh</a> == <a href="ident?i=NULL">NULL</a>) {
 <a name="L35" href="source/fs/qnx4/dir.c#L35">35</a>                         <a href="ident?i=printk">printk</a>(<a href="ident?i=KERN_ERR">KERN_ERR</a> <i>"qnx4_readdir: bread failed (%ld)\n"</i>, blknum);
 <a name="L36" href="source/fs/qnx4/dir.c#L36">36</a>                         return 0;
 <a name="L37" href="source/fs/qnx4/dir.c#L37">37</a>                 }
 <a name="L38" href="source/fs/qnx4/dir.c#L38">38</a>                 ix = (<a href="ident?i=ctx">ctx</a>-&gt;<a href="ident?i=pos">pos</a> &gt;&gt; <a href="ident?i=QNX4_DIR_ENTRY_SIZE_BITS">QNX4_DIR_ENTRY_SIZE_BITS</a>) % <a href="ident?i=QNX4_INODES_PER_BLOCK">QNX4_INODES_PER_BLOCK</a>;
 <a name="L39" href="source/fs/qnx4/dir.c#L39">39</a>                 for (; ix &lt; <a href="ident?i=QNX4_INODES_PER_BLOCK">QNX4_INODES_PER_BLOCK</a>; ix++, <a href="ident?i=ctx">ctx</a>-&gt;<a href="ident?i=pos">pos</a> += <a href="ident?i=QNX4_DIR_ENTRY_SIZE">QNX4_DIR_ENTRY_SIZE</a>) {
 <a name="L40" href="source/fs/qnx4/dir.c#L40">40</a>                         <a href="ident?i=offset">offset</a> = ix * <a href="ident?i=QNX4_DIR_ENTRY_SIZE">QNX4_DIR_ENTRY_SIZE</a>;
 <a name="L41" href="source/fs/qnx4/dir.c#L41">41</a>                         <a href="ident?i=de">de</a> = (struct <a href="ident?i=qnx4_inode_entry">qnx4_inode_entry</a> *) (<a href="ident?i=bh">bh</a>-&gt;b_data + <a href="ident?i=offset">offset</a>);
 <a name="L42" href="source/fs/qnx4/dir.c#L42">42</a>                         if (!<a href="ident?i=de">de</a>-&gt;di_fname[0])
 <a name="L43" href="source/fs/qnx4/dir.c#L43">43</a>                                 continue;
 <a name="L44" href="source/fs/qnx4/dir.c#L44">44</a>                         if (!(<a href="ident?i=de">de</a>-&gt;di_status &amp; (<a href="ident?i=QNX4_FILE_USED">QNX4_FILE_USED</a>|<a href="ident?i=QNX4_FILE_LINK">QNX4_FILE_LINK</a>)))
 <a name="L45" href="source/fs/qnx4/dir.c#L45">45</a>                                 continue;
 <a name="L46" href="source/fs/qnx4/dir.c#L46">46</a>                         if (!(<a href="ident?i=de">de</a>-&gt;di_status &amp; <a href="ident?i=QNX4_FILE_LINK">QNX4_FILE_LINK</a>))
 <a name="L47" href="source/fs/qnx4/dir.c#L47">47</a>                                 <a href="ident?i=size">size</a> = <a href="ident?i=QNX4_SHORT_NAME_MAX">QNX4_SHORT_NAME_MAX</a>;
 <a name="L48" href="source/fs/qnx4/dir.c#L48">48</a>                         else
 <a name="L49" href="source/fs/qnx4/dir.c#L49">49</a>                                 <a href="ident?i=size">size</a> = <a href="ident?i=QNX4_NAME_MAX">QNX4_NAME_MAX</a>;
 <a name="L50" href="source/fs/qnx4/dir.c#L50">50</a>                         <a href="ident?i=size">size</a> = <a href="ident?i=strnlen">strnlen</a>(<a href="ident?i=de">de</a>-&gt;di_fname, <a href="ident?i=size">size</a>);
 <a name="L51" href="source/fs/qnx4/dir.c#L51">51</a>                         <a href="ident?i=QNX4DEBUG">QNX4DEBUG</a>((<a href="ident?i=KERN_INFO">KERN_INFO</a> <i>"qnx4_readdir:%.*s\n"</i>, <a href="ident?i=size">size</a>, <a href="ident?i=de">de</a>-&gt;di_fname));
 <a name="L52" href="source/fs/qnx4/dir.c#L52">52</a>                         if (!(<a href="ident?i=de">de</a>-&gt;di_status &amp; <a href="ident?i=QNX4_FILE_LINK">QNX4_FILE_LINK</a>))
 <a name="L53" href="source/fs/qnx4/dir.c#L53">53</a>                                 <a href="ident?i=ino">ino</a> = blknum * <a href="ident?i=QNX4_INODES_PER_BLOCK">QNX4_INODES_PER_BLOCK</a> + ix - 1;
 <a name="L54" href="source/fs/qnx4/dir.c#L54">54</a>                         else {
 <a name="L55" href="source/fs/qnx4/dir.c#L55">55</a>                                 le  = (struct <a href="ident?i=qnx4_link_info">qnx4_link_info</a>*)<a href="ident?i=de">de</a>;
 <a name="L56" href="source/fs/qnx4/dir.c#L56">56</a>                                 <a href="ident?i=ino">ino</a> = ( <a href="ident?i=le32_to_cpu">le32_to_cpu</a>(le-&gt;dl_inode_blk) - 1 ) *
 <a name="L57" href="source/fs/qnx4/dir.c#L57">57</a>                                         <a href="ident?i=QNX4_INODES_PER_BLOCK">QNX4_INODES_PER_BLOCK</a> +
 <a name="L58" href="source/fs/qnx4/dir.c#L58">58</a>                                         le-&gt;dl_inode_ndx;
 <a name="L59" href="source/fs/qnx4/dir.c#L59">59</a>                         }
 <a name="L60" href="source/fs/qnx4/dir.c#L60">60</a>                         if (!<a href="ident?i=dir_emit">dir_emit</a>(<a href="ident?i=ctx">ctx</a>, <a href="ident?i=de">de</a>-&gt;di_fname, <a href="ident?i=size">size</a>, <a href="ident?i=ino">ino</a>, <a href="ident?i=DT_UNKNOWN">DT_UNKNOWN</a>)) {
 <a name="L61" href="source/fs/qnx4/dir.c#L61">61</a>                                 <a href="ident?i=brelse">brelse</a>(<a href="ident?i=bh">bh</a>);
 <a name="L62" href="source/fs/qnx4/dir.c#L62">62</a>                                 return 0;
 <a name="L63" href="source/fs/qnx4/dir.c#L63">63</a>                         }
 <a name="L64" href="source/fs/qnx4/dir.c#L64">64</a>                 }
 <a name="L65" href="source/fs/qnx4/dir.c#L65">65</a>                 <a href="ident?i=brelse">brelse</a>(<a href="ident?i=bh">bh</a>);
 <a name="L66" href="source/fs/qnx4/dir.c#L66">66</a>         }
 <a name="L67" href="source/fs/qnx4/dir.c#L67">67</a>         return 0;
 <a name="L68" href="source/fs/qnx4/dir.c#L68">68</a> }
 <a name="L69" href="source/fs/qnx4/dir.c#L69">69</a> 
 <a name="L70" href="source/fs/qnx4/dir.c#L70">70</a> const struct <a href="ident?i=file_operations">file_operations</a> <a href="ident?i=qnx4_dir_operations">qnx4_dir_operations</a> =
 <a name="L71" href="source/fs/qnx4/dir.c#L71">71</a> {
 <a name="L72" href="source/fs/qnx4/dir.c#L72">72</a>         .llseek         = <a href="ident?i=generic_file_llseek">generic_file_llseek</a>,
 <a name="L73" href="source/fs/qnx4/dir.c#L73">73</a>         .<a href="ident?i=read">read</a>           = <a href="ident?i=generic_read_dir">generic_read_dir</a>,
 <a name="L74" href="source/fs/qnx4/dir.c#L74">74</a>         .iterate        = <a href="ident?i=qnx4_readdir">qnx4_readdir</a>,
 <a name="L75" href="source/fs/qnx4/dir.c#L75">75</a>         .fsync          = <a href="ident?i=generic_file_fsync">generic_file_fsync</a>,
 <a name="L76" href="source/fs/qnx4/dir.c#L76">76</a> };
 <a name="L77" href="source/fs/qnx4/dir.c#L77">77</a> 
 <a name="L78" href="source/fs/qnx4/dir.c#L78">78</a> const struct <a href="ident?i=inode_operations">inode_operations</a> <a href="ident?i=qnx4_dir_inode_operations">qnx4_dir_inode_operations</a> =
 <a name="L79" href="source/fs/qnx4/dir.c#L79">79</a> {
 <a name="L80" href="source/fs/qnx4/dir.c#L80">80</a>         .<a href="ident?i=lookup">lookup</a>         = <a href="ident?i=qnx4_lookup">qnx4_lookup</a>,
 <a name="L81" href="source/fs/qnx4/dir.c#L81">81</a> };
 <a name="L82" href="source/fs/qnx4/dir.c#L82">82</a> </pre></div><p>
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
