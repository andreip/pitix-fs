<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<title>Linux/fs/qnx4/namei.c - Linux Cross Reference - Free Electrons</title>
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
		 &nbsp;&bull;&nbsp;<b><i>source navigation</i></b> &nbsp;&bull;&nbsp;<a href="diff/fs/qnx4/namei.c">diff markup</a> &nbsp;&bull;&nbsp;<a href="ident">identifier search</a> &nbsp;&bull;&nbsp;<a href="search">freetext search</a> &nbsp;&bull;&nbsp;
		</p>
	</div>
</div>

<div id="topbar">
  
  <p>Version: &nbsp;<a href="source/fs/qnx4/namei.c?v=2.6.34">2.6.34</a>&nbsp;<a href="source/fs/qnx4/namei.c?v=2.6.35">2.6.35</a>&nbsp;<a href="source/fs/qnx4/namei.c?v=2.6.36">2.6.36</a>&nbsp;<a href="source/fs/qnx4/namei.c?v=2.6.37">2.6.37</a>&nbsp;<a href="source/fs/qnx4/namei.c?v=2.6.38">2.6.38</a>&nbsp;<a href="source/fs/qnx4/namei.c?v=2.6.39">2.6.39</a>&nbsp;<a href="source/fs/qnx4/namei.c?v=3.0">3.0</a>&nbsp;<a href="source/fs/qnx4/namei.c?v=3.1">3.1</a>&nbsp;<a href="source/fs/qnx4/namei.c?v=3.2">3.2</a>&nbsp;<a href="source/fs/qnx4/namei.c?v=3.3">3.3</a>&nbsp;<a href="source/fs/qnx4/namei.c?v=3.4">3.4</a>&nbsp;<a href="source/fs/qnx4/namei.c?v=3.5">3.5</a>&nbsp;<a href="source/fs/qnx4/namei.c?v=3.6">3.6</a>&nbsp;<a href="source/fs/qnx4/namei.c?v=3.7">3.7</a>&nbsp;<a href="source/fs/qnx4/namei.c?v=3.8">3.8</a>&nbsp;<a href="source/fs/qnx4/namei.c?v=3.9">3.9</a>&nbsp;<a href="source/fs/qnx4/namei.c?v=3.10">3.10</a>&nbsp;<a href="source/fs/qnx4/namei.c?v=3.11">3.11</a>&nbsp;<a href="source/fs/qnx4/namei.c?v=3.12">3.12</a>&nbsp;<a href="source/fs/qnx4/namei.c?v=3.13">3.13</a>&nbsp;<b><i>3.14</i></b></p>
  
</div>

<h1><a href="source/">Linux</a>/<a href="source/fs/">fs</a>/<a href="source/fs/qnx4/">qnx4</a>/<a href="source/fs/qnx4/namei.c">namei.c</a></h1>
<div id="lxrcode"><pre>  <a name="L1" href="source/fs/qnx4/namei.c#L1">1</a> <b><i>/* </i></b>
  <a name="L2" href="source/fs/qnx4/namei.c#L2">2</a> <b><i> * QNX4 file system, Linux implementation.</i></b>
  <a name="L3" href="source/fs/qnx4/namei.c#L3">3</a> <b><i> * </i></b>
  <a name="L4" href="source/fs/qnx4/namei.c#L4">4</a> <b><i> * Version : 0.2.1</i></b>
  <a name="L5" href="source/fs/qnx4/namei.c#L5">5</a> <b><i> * </i></b>
  <a name="L6" href="source/fs/qnx4/namei.c#L6">6</a> <b><i> * Using parts of the xiafs filesystem.</i></b>
  <a name="L7" href="source/fs/qnx4/namei.c#L7">7</a> <b><i> * </i></b>
  <a name="L8" href="source/fs/qnx4/namei.c#L8">8</a> <b><i> * History :</i></b>
  <a name="L9" href="source/fs/qnx4/namei.c#L9">9</a> <b><i> * </i></b>
 <a name="L10" href="source/fs/qnx4/namei.c#L10">10</a> <b><i> * 01-06-1998 by Richard Frowijn : first release.</i></b>
 <a name="L11" href="source/fs/qnx4/namei.c#L11">11</a> <b><i> * 21-06-1998 by Frank Denis : dcache support, fixed error codes.</i></b>
 <a name="L12" href="source/fs/qnx4/namei.c#L12">12</a> <b><i> * 04-07-1998 by Frank Denis : first step for rmdir/unlink.</i></b>
 <a name="L13" href="source/fs/qnx4/namei.c#L13">13</a> <b><i> */</i></b>
 <a name="L14" href="source/fs/qnx4/namei.c#L14">14</a> 
 <a name="L15" href="source/fs/qnx4/namei.c#L15">15</a> #include &lt;linux/buffer_head.h&gt;
 <a name="L16" href="source/fs/qnx4/namei.c#L16">16</a> #include "<a href="source/fs/qnx4/qnx4.h">qnx4.h</a>"
 <a name="L17" href="source/fs/qnx4/namei.c#L17">17</a> 
 <a name="L18" href="source/fs/qnx4/namei.c#L18">18</a> 
 <a name="L19" href="source/fs/qnx4/namei.c#L19">19</a> <b><i>/*</i></b>
 <a name="L20" href="source/fs/qnx4/namei.c#L20">20</a> <b><i> * check if the filename is correct. For some obscure reason, qnx writes a</i></b>
 <a name="L21" href="source/fs/qnx4/namei.c#L21">21</a> <b><i> * new file twice in the directory entry, first with all possible options at 0</i></b>
 <a name="L22" href="source/fs/qnx4/namei.c#L22">22</a> <b><i> * and for a second time the way it is, they want us not to access the qnx</i></b>
 <a name="L23" href="source/fs/qnx4/namei.c#L23">23</a> <b><i> * filesystem when whe are using linux.</i></b>
 <a name="L24" href="source/fs/qnx4/namei.c#L24">24</a> <b><i> */</i></b>
 <a name="L25" href="source/fs/qnx4/namei.c#L25">25</a> static int <a href="ident?i=qnx4_match">qnx4_match</a>(int <a href="ident?i=len">len</a>, const char *<a href="ident?i=name">name</a>,
 <a name="L26" href="source/fs/qnx4/namei.c#L26">26</a>                       struct <a href="ident?i=buffer_head">buffer_head</a> *<a href="ident?i=bh">bh</a>, unsigned long *<a href="ident?i=offset">offset</a>)
 <a name="L27" href="source/fs/qnx4/namei.c#L27">27</a> {
 <a name="L28" href="source/fs/qnx4/namei.c#L28">28</a>         struct <a href="ident?i=qnx4_inode_entry">qnx4_inode_entry</a> *<a href="ident?i=de">de</a>;
 <a name="L29" href="source/fs/qnx4/namei.c#L29">29</a>         int namelen, thislen;
 <a name="L30" href="source/fs/qnx4/namei.c#L30">30</a> 
 <a name="L31" href="source/fs/qnx4/namei.c#L31">31</a>         if (<a href="ident?i=bh">bh</a> == <a href="ident?i=NULL">NULL</a>) {
 <a name="L32" href="source/fs/qnx4/namei.c#L32">32</a>                 <a href="ident?i=printk">printk</a>(<a href="ident?i=KERN_WARNING">KERN_WARNING</a> <i>"qnx4: matching unassigned buffer !\n"</i>);
 <a name="L33" href="source/fs/qnx4/namei.c#L33">33</a>                 return 0;
 <a name="L34" href="source/fs/qnx4/namei.c#L34">34</a>         }
 <a name="L35" href="source/fs/qnx4/namei.c#L35">35</a>         <a href="ident?i=de">de</a> = (struct <a href="ident?i=qnx4_inode_entry">qnx4_inode_entry</a> *) (<a href="ident?i=bh">bh</a>-&gt;b_data + *<a href="ident?i=offset">offset</a>);
 <a name="L36" href="source/fs/qnx4/namei.c#L36">36</a>         *<a href="ident?i=offset">offset</a> += <a href="ident?i=QNX4_DIR_ENTRY_SIZE">QNX4_DIR_ENTRY_SIZE</a>;
 <a name="L37" href="source/fs/qnx4/namei.c#L37">37</a>         if ((<a href="ident?i=de">de</a>-&gt;di_status &amp; <a href="ident?i=QNX4_FILE_LINK">QNX4_FILE_LINK</a>) != 0) {
 <a name="L38" href="source/fs/qnx4/namei.c#L38">38</a>                 namelen = <a href="ident?i=QNX4_NAME_MAX">QNX4_NAME_MAX</a>;
 <a name="L39" href="source/fs/qnx4/namei.c#L39">39</a>         } else {
 <a name="L40" href="source/fs/qnx4/namei.c#L40">40</a>                 namelen = <a href="ident?i=QNX4_SHORT_NAME_MAX">QNX4_SHORT_NAME_MAX</a>;
 <a name="L41" href="source/fs/qnx4/namei.c#L41">41</a>         }
 <a name="L42" href="source/fs/qnx4/namei.c#L42">42</a>         thislen = <a href="ident?i=strlen">strlen</a>( <a href="ident?i=de">de</a>-&gt;di_fname );
 <a name="L43" href="source/fs/qnx4/namei.c#L43">43</a>         if ( thislen &gt; namelen )
 <a name="L44" href="source/fs/qnx4/namei.c#L44">44</a>                 thislen = namelen;
 <a name="L45" href="source/fs/qnx4/namei.c#L45">45</a>         if (<a href="ident?i=len">len</a> != thislen) {
 <a name="L46" href="source/fs/qnx4/namei.c#L46">46</a>                 return 0;
 <a name="L47" href="source/fs/qnx4/namei.c#L47">47</a>         }
 <a name="L48" href="source/fs/qnx4/namei.c#L48">48</a>         if (<a href="ident?i=strncmp">strncmp</a>(<a href="ident?i=name">name</a>, <a href="ident?i=de">de</a>-&gt;di_fname, <a href="ident?i=len">len</a>) == 0) {
 <a name="L49" href="source/fs/qnx4/namei.c#L49">49</a>                 if ((<a href="ident?i=de">de</a>-&gt;di_status &amp; (<a href="ident?i=QNX4_FILE_USED">QNX4_FILE_USED</a>|<a href="ident?i=QNX4_FILE_LINK">QNX4_FILE_LINK</a>)) != 0) {
 <a name="L50" href="source/fs/qnx4/namei.c#L50">50</a>                         return 1;
 <a name="L51" href="source/fs/qnx4/namei.c#L51">51</a>                 }
 <a name="L52" href="source/fs/qnx4/namei.c#L52">52</a>         }
 <a name="L53" href="source/fs/qnx4/namei.c#L53">53</a>         return 0;
 <a name="L54" href="source/fs/qnx4/namei.c#L54">54</a> }
 <a name="L55" href="source/fs/qnx4/namei.c#L55">55</a> 
 <a name="L56" href="source/fs/qnx4/namei.c#L56">56</a> static struct <a href="ident?i=buffer_head">buffer_head</a> *<a href="ident?i=qnx4_find_entry">qnx4_find_entry</a>(int <a href="ident?i=len">len</a>, struct <a href="ident?i=inode">inode</a> *<a href="ident?i=dir">dir</a>,
 <a name="L57" href="source/fs/qnx4/namei.c#L57">57</a>            const char *<a href="ident?i=name">name</a>, struct <a href="ident?i=qnx4_inode_entry">qnx4_inode_entry</a> **res_dir, int *<a href="ident?i=ino">ino</a>)
 <a name="L58" href="source/fs/qnx4/namei.c#L58">58</a> {
 <a name="L59" href="source/fs/qnx4/namei.c#L59">59</a>         unsigned long <a href="ident?i=block">block</a>, <a href="ident?i=offset">offset</a>, blkofs;
 <a name="L60" href="source/fs/qnx4/namei.c#L60">60</a>         struct <a href="ident?i=buffer_head">buffer_head</a> *<a href="ident?i=bh">bh</a>;
 <a name="L61" href="source/fs/qnx4/namei.c#L61">61</a> 
 <a name="L62" href="source/fs/qnx4/namei.c#L62">62</a>         *res_dir = <a href="ident?i=NULL">NULL</a>;
 <a name="L63" href="source/fs/qnx4/namei.c#L63">63</a>         <a href="ident?i=bh">bh</a> = <a href="ident?i=NULL">NULL</a>;
 <a name="L64" href="source/fs/qnx4/namei.c#L64">64</a>         <a href="ident?i=block">block</a> = <a href="ident?i=offset">offset</a> = blkofs = 0;
 <a name="L65" href="source/fs/qnx4/namei.c#L65">65</a>         while (blkofs * <a href="ident?i=QNX4_BLOCK_SIZE">QNX4_BLOCK_SIZE</a> + <a href="ident?i=offset">offset</a> &lt; <a href="ident?i=dir">dir</a>-&gt;i_size) {
 <a name="L66" href="source/fs/qnx4/namei.c#L66">66</a>                 if (!<a href="ident?i=bh">bh</a>) {
 <a name="L67" href="source/fs/qnx4/namei.c#L67">67</a>                         <a href="ident?i=block">block</a> = <a href="ident?i=qnx4_block_map">qnx4_block_map</a>(<a href="ident?i=dir">dir</a>, blkofs);
 <a name="L68" href="source/fs/qnx4/namei.c#L68">68</a>                         if (<a href="ident?i=block">block</a>)
 <a name="L69" href="source/fs/qnx4/namei.c#L69">69</a>                                 <a href="ident?i=bh">bh</a> = <a href="ident?i=sb_bread">sb_bread</a>(<a href="ident?i=dir">dir</a>-&gt;i_sb, <a href="ident?i=block">block</a>);
 <a name="L70" href="source/fs/qnx4/namei.c#L70">70</a>                         if (!<a href="ident?i=bh">bh</a>) {
 <a name="L71" href="source/fs/qnx4/namei.c#L71">71</a>                                 blkofs++;
 <a name="L72" href="source/fs/qnx4/namei.c#L72">72</a>                                 continue;
 <a name="L73" href="source/fs/qnx4/namei.c#L73">73</a>                         }
 <a name="L74" href="source/fs/qnx4/namei.c#L74">74</a>                 }
 <a name="L75" href="source/fs/qnx4/namei.c#L75">75</a>                 *res_dir = (struct <a href="ident?i=qnx4_inode_entry">qnx4_inode_entry</a> *) (<a href="ident?i=bh">bh</a>-&gt;b_data + <a href="ident?i=offset">offset</a>);
 <a name="L76" href="source/fs/qnx4/namei.c#L76">76</a>                 if (<a href="ident?i=qnx4_match">qnx4_match</a>(<a href="ident?i=len">len</a>, <a href="ident?i=name">name</a>, <a href="ident?i=bh">bh</a>, &amp;<a href="ident?i=offset">offset</a>)) {
 <a name="L77" href="source/fs/qnx4/namei.c#L77">77</a>                         *<a href="ident?i=ino">ino</a> = <a href="ident?i=block">block</a> * <a href="ident?i=QNX4_INODES_PER_BLOCK">QNX4_INODES_PER_BLOCK</a> +
 <a name="L78" href="source/fs/qnx4/namei.c#L78">78</a>                             (<a href="ident?i=offset">offset</a> / <a href="ident?i=QNX4_DIR_ENTRY_SIZE">QNX4_DIR_ENTRY_SIZE</a>) - 1;
 <a name="L79" href="source/fs/qnx4/namei.c#L79">79</a>                         return <a href="ident?i=bh">bh</a>;
 <a name="L80" href="source/fs/qnx4/namei.c#L80">80</a>                 }
 <a name="L81" href="source/fs/qnx4/namei.c#L81">81</a>                 if (<a href="ident?i=offset">offset</a> &lt; <a href="ident?i=bh">bh</a>-&gt;b_size) {
 <a name="L82" href="source/fs/qnx4/namei.c#L82">82</a>                         continue;
 <a name="L83" href="source/fs/qnx4/namei.c#L83">83</a>                 }
 <a name="L84" href="source/fs/qnx4/namei.c#L84">84</a>                 <a href="ident?i=brelse">brelse</a>(<a href="ident?i=bh">bh</a>);
 <a name="L85" href="source/fs/qnx4/namei.c#L85">85</a>                 <a href="ident?i=bh">bh</a> = <a href="ident?i=NULL">NULL</a>;
 <a name="L86" href="source/fs/qnx4/namei.c#L86">86</a>                 <a href="ident?i=offset">offset</a> = 0;
 <a name="L87" href="source/fs/qnx4/namei.c#L87">87</a>                 blkofs++;
 <a name="L88" href="source/fs/qnx4/namei.c#L88">88</a>         }
 <a name="L89" href="source/fs/qnx4/namei.c#L89">89</a>         <a href="ident?i=brelse">brelse</a>(<a href="ident?i=bh">bh</a>);
 <a name="L90" href="source/fs/qnx4/namei.c#L90">90</a>         *res_dir = <a href="ident?i=NULL">NULL</a>;
 <a name="L91" href="source/fs/qnx4/namei.c#L91">91</a>         return <a href="ident?i=NULL">NULL</a>;
 <a name="L92" href="source/fs/qnx4/namei.c#L92">92</a> }
 <a name="L93" href="source/fs/qnx4/namei.c#L93">93</a> 
 <a name="L94" href="source/fs/qnx4/namei.c#L94">94</a> struct <a href="ident?i=dentry">dentry</a> * <a href="ident?i=qnx4_lookup">qnx4_lookup</a>(struct <a href="ident?i=inode">inode</a> *<a href="ident?i=dir">dir</a>, struct <a href="ident?i=dentry">dentry</a> *<a href="ident?i=dentry">dentry</a>, unsigned int <a href="ident?i=flags">flags</a>)
 <a name="L95" href="source/fs/qnx4/namei.c#L95">95</a> {
 <a name="L96" href="source/fs/qnx4/namei.c#L96">96</a>         int <a href="ident?i=ino">ino</a>;
 <a name="L97" href="source/fs/qnx4/namei.c#L97">97</a>         struct <a href="ident?i=qnx4_inode_entry">qnx4_inode_entry</a> *<a href="ident?i=de">de</a>;
 <a name="L98" href="source/fs/qnx4/namei.c#L98">98</a>         struct <a href="ident?i=qnx4_link_info">qnx4_link_info</a> *lnk;
 <a name="L99" href="source/fs/qnx4/namei.c#L99">99</a>         struct <a href="ident?i=buffer_head">buffer_head</a> *<a href="ident?i=bh">bh</a>;
<a name="L100" href="source/fs/qnx4/namei.c#L100">100</a>         const char *<a href="ident?i=name">name</a> = <a href="ident?i=dentry">dentry</a>-&gt;d_name.<a href="ident?i=name">name</a>;
<a name="L101" href="source/fs/qnx4/namei.c#L101">101</a>         int <a href="ident?i=len">len</a> = <a href="ident?i=dentry">dentry</a>-&gt;d_name.<a href="ident?i=len">len</a>;
<a name="L102" href="source/fs/qnx4/namei.c#L102">102</a>         struct <a href="ident?i=inode">inode</a> *foundinode = <a href="ident?i=NULL">NULL</a>;
<a name="L103" href="source/fs/qnx4/namei.c#L103">103</a> 
<a name="L104" href="source/fs/qnx4/namei.c#L104">104</a>         if (!(<a href="ident?i=bh">bh</a> = <a href="ident?i=qnx4_find_entry">qnx4_find_entry</a>(<a href="ident?i=len">len</a>, <a href="ident?i=dir">dir</a>, <a href="ident?i=name">name</a>, &amp;<a href="ident?i=de">de</a>, &amp;<a href="ident?i=ino">ino</a>)))
<a name="L105" href="source/fs/qnx4/namei.c#L105">105</a>                 goto <a href="ident?i=out">out</a>;
<a name="L106" href="source/fs/qnx4/namei.c#L106">106</a>         <b><i>/* The entry is linked, let's get the real info */</i></b>
<a name="L107" href="source/fs/qnx4/namei.c#L107">107</a>         if ((<a href="ident?i=de">de</a>-&gt;di_status &amp; <a href="ident?i=QNX4_FILE_LINK">QNX4_FILE_LINK</a>) == <a href="ident?i=QNX4_FILE_LINK">QNX4_FILE_LINK</a>) {
<a name="L108" href="source/fs/qnx4/namei.c#L108">108</a>                 lnk = (struct <a href="ident?i=qnx4_link_info">qnx4_link_info</a> *) <a href="ident?i=de">de</a>;
<a name="L109" href="source/fs/qnx4/namei.c#L109">109</a>                 <a href="ident?i=ino">ino</a> = (<a href="ident?i=le32_to_cpu">le32_to_cpu</a>(lnk-&gt;dl_inode_blk) - 1) *
<a name="L110" href="source/fs/qnx4/namei.c#L110">110</a>                     <a href="ident?i=QNX4_INODES_PER_BLOCK">QNX4_INODES_PER_BLOCK</a> +
<a name="L111" href="source/fs/qnx4/namei.c#L111">111</a>                     lnk-&gt;dl_inode_ndx;
<a name="L112" href="source/fs/qnx4/namei.c#L112">112</a>         }
<a name="L113" href="source/fs/qnx4/namei.c#L113">113</a>         <a href="ident?i=brelse">brelse</a>(<a href="ident?i=bh">bh</a>);
<a name="L114" href="source/fs/qnx4/namei.c#L114">114</a> 
<a name="L115" href="source/fs/qnx4/namei.c#L115">115</a>         foundinode = <a href="ident?i=qnx4_iget">qnx4_iget</a>(<a href="ident?i=dir">dir</a>-&gt;i_sb, <a href="ident?i=ino">ino</a>);
<a name="L116" href="source/fs/qnx4/namei.c#L116">116</a>         if (<a href="ident?i=IS_ERR">IS_ERR</a>(foundinode)) {
<a name="L117" href="source/fs/qnx4/namei.c#L117">117</a>                 <a href="ident?i=QNX4DEBUG">QNX4DEBUG</a>((<a href="ident?i=KERN_ERR">KERN_ERR</a> <i>"qnx4: lookup-&gt;iget -&gt; error %ld\n"</i>,
<a name="L118" href="source/fs/qnx4/namei.c#L118">118</a>                            <a href="ident?i=PTR_ERR">PTR_ERR</a>(foundinode)));
<a name="L119" href="source/fs/qnx4/namei.c#L119">119</a>                 return <a href="ident?i=ERR_CAST">ERR_CAST</a>(foundinode);
<a name="L120" href="source/fs/qnx4/namei.c#L120">120</a>         }
<a name="L121" href="source/fs/qnx4/namei.c#L121">121</a> <a href="ident?i=out">out</a>:
<a name="L122" href="source/fs/qnx4/namei.c#L122">122</a>         <a href="ident?i=d_add">d_add</a>(<a href="ident?i=dentry">dentry</a>, foundinode);
<a name="L123" href="source/fs/qnx4/namei.c#L123">123</a> 
<a name="L124" href="source/fs/qnx4/namei.c#L124">124</a>         return <a href="ident?i=NULL">NULL</a>;
<a name="L125" href="source/fs/qnx4/namei.c#L125">125</a> }
<a name="L126" href="source/fs/qnx4/namei.c#L126">126</a> </pre></div><p>
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
