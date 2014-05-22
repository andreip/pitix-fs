<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<title>Linux/fs/qnx4/inode.c - Linux Cross Reference - Free Electrons</title>
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
		 &nbsp;&bull;&nbsp;<b><i>source navigation</i></b> &nbsp;&bull;&nbsp;<a href="diff/fs/qnx4/inode.c">diff markup</a> &nbsp;&bull;&nbsp;<a href="ident">identifier search</a> &nbsp;&bull;&nbsp;<a href="search">freetext search</a> &nbsp;&bull;&nbsp;
		</p>
	</div>
</div>

<div id="topbar">
  
  <p>Version: &nbsp;<a href="source/fs/qnx4/inode.c?v=2.6.34">2.6.34</a>&nbsp;<a href="source/fs/qnx4/inode.c?v=2.6.35">2.6.35</a>&nbsp;<a href="source/fs/qnx4/inode.c?v=2.6.36">2.6.36</a>&nbsp;<a href="source/fs/qnx4/inode.c?v=2.6.37">2.6.37</a>&nbsp;<a href="source/fs/qnx4/inode.c?v=2.6.38">2.6.38</a>&nbsp;<a href="source/fs/qnx4/inode.c?v=2.6.39">2.6.39</a>&nbsp;<a href="source/fs/qnx4/inode.c?v=3.0">3.0</a>&nbsp;<a href="source/fs/qnx4/inode.c?v=3.1">3.1</a>&nbsp;<a href="source/fs/qnx4/inode.c?v=3.2">3.2</a>&nbsp;<a href="source/fs/qnx4/inode.c?v=3.3">3.3</a>&nbsp;<a href="source/fs/qnx4/inode.c?v=3.4">3.4</a>&nbsp;<a href="source/fs/qnx4/inode.c?v=3.5">3.5</a>&nbsp;<a href="source/fs/qnx4/inode.c?v=3.6">3.6</a>&nbsp;<a href="source/fs/qnx4/inode.c?v=3.7">3.7</a>&nbsp;<a href="source/fs/qnx4/inode.c?v=3.8">3.8</a>&nbsp;<a href="source/fs/qnx4/inode.c?v=3.9">3.9</a>&nbsp;<a href="source/fs/qnx4/inode.c?v=3.10">3.10</a>&nbsp;<a href="source/fs/qnx4/inode.c?v=3.11">3.11</a>&nbsp;<a href="source/fs/qnx4/inode.c?v=3.12">3.12</a>&nbsp;<a href="source/fs/qnx4/inode.c?v=3.13">3.13</a>&nbsp;<b><i>3.14</i></b></p>
  
</div>

<h1><a href="source/">Linux</a>/<a href="source/fs/">fs</a>/<a href="source/fs/qnx4/">qnx4</a>/<a href="source/fs/qnx4/inode.c">inode.c</a></h1>
<div id="lxrcode"><pre>  <a name="L1" href="source/fs/qnx4/inode.c#L1">1</a> <b><i>/*</i></b>
  <a name="L2" href="source/fs/qnx4/inode.c#L2">2</a> <b><i> * QNX4 file system, Linux implementation.</i></b>
  <a name="L3" href="source/fs/qnx4/inode.c#L3">3</a> <b><i> *</i></b>
  <a name="L4" href="source/fs/qnx4/inode.c#L4">4</a> <b><i> * Version : 0.2.1</i></b>
  <a name="L5" href="source/fs/qnx4/inode.c#L5">5</a> <b><i> *</i></b>
  <a name="L6" href="source/fs/qnx4/inode.c#L6">6</a> <b><i> * Using parts of the xiafs filesystem.</i></b>
  <a name="L7" href="source/fs/qnx4/inode.c#L7">7</a> <b><i> *</i></b>
  <a name="L8" href="source/fs/qnx4/inode.c#L8">8</a> <b><i> * History :</i></b>
  <a name="L9" href="source/fs/qnx4/inode.c#L9">9</a> <b><i> *</i></b>
 <a name="L10" href="source/fs/qnx4/inode.c#L10">10</a> <b><i> * 01-06-1998 by Richard Frowijn : first release.</i></b>
 <a name="L11" href="source/fs/qnx4/inode.c#L11">11</a> <b><i> * 20-06-1998 by Frank Denis : Linux 2.1.99+ support, boot signature, misc.</i></b>
 <a name="L12" href="source/fs/qnx4/inode.c#L12">12</a> <b><i> * 30-06-1998 by Frank Denis : first step to write inodes.</i></b>
 <a name="L13" href="source/fs/qnx4/inode.c#L13">13</a> <b><i> */</i></b>
 <a name="L14" href="source/fs/qnx4/inode.c#L14">14</a> 
 <a name="L15" href="source/fs/qnx4/inode.c#L15">15</a> #include &lt;linux/module.h&gt;
 <a name="L16" href="source/fs/qnx4/inode.c#L16">16</a> #include &lt;linux/init.h&gt;
 <a name="L17" href="source/fs/qnx4/inode.c#L17">17</a> #include &lt;linux/slab.h&gt;
 <a name="L18" href="source/fs/qnx4/inode.c#L18">18</a> #include &lt;linux/highuid.h&gt;
 <a name="L19" href="source/fs/qnx4/inode.c#L19">19</a> #include &lt;linux/pagemap.h&gt;
 <a name="L20" href="source/fs/qnx4/inode.c#L20">20</a> #include &lt;linux/buffer_head.h&gt;
 <a name="L21" href="source/fs/qnx4/inode.c#L21">21</a> #include &lt;linux/writeback.h&gt;
 <a name="L22" href="source/fs/qnx4/inode.c#L22">22</a> #include &lt;linux/statfs.h&gt;
 <a name="L23" href="source/fs/qnx4/inode.c#L23">23</a> #include "<a href="source/fs/qnx4/qnx4.h">qnx4.h</a>"
 <a name="L24" href="source/fs/qnx4/inode.c#L24">24</a> 
 <a name="L25" href="source/fs/qnx4/inode.c#L25">25</a> #define <a href="ident?i=QNX4_VERSION">QNX4_VERSION</a>  4
 <a name="L26" href="source/fs/qnx4/inode.c#L26">26</a> #define <a href="ident?i=QNX4_BMNAME">QNX4_BMNAME</a>   <i>".bitmap"</i>
 <a name="L27" href="source/fs/qnx4/inode.c#L27">27</a> 
 <a name="L28" href="source/fs/qnx4/inode.c#L28">28</a> static const struct <a href="ident?i=super_operations">super_operations</a> <a href="ident?i=qnx4_sops">qnx4_sops</a>;
 <a name="L29" href="source/fs/qnx4/inode.c#L29">29</a> 
 <a name="L30" href="source/fs/qnx4/inode.c#L30">30</a> static struct <a href="ident?i=inode">inode</a> *<a href="ident?i=qnx4_alloc_inode">qnx4_alloc_inode</a>(struct <a href="ident?i=super_block">super_block</a> *<a href="ident?i=sb">sb</a>);
 <a name="L31" href="source/fs/qnx4/inode.c#L31">31</a> static void <a href="ident?i=qnx4_destroy_inode">qnx4_destroy_inode</a>(struct <a href="ident?i=inode">inode</a> *<a href="ident?i=inode">inode</a>);
 <a name="L32" href="source/fs/qnx4/inode.c#L32">32</a> static int <a href="ident?i=qnx4_remount">qnx4_remount</a>(struct <a href="ident?i=super_block">super_block</a> *<a href="ident?i=sb">sb</a>, int *<a href="ident?i=flags">flags</a>, char *<a href="ident?i=data">data</a>);
 <a name="L33" href="source/fs/qnx4/inode.c#L33">33</a> static int <a href="ident?i=qnx4_statfs">qnx4_statfs</a>(struct <a href="ident?i=dentry">dentry</a> *, struct <a href="ident?i=kstatfs">kstatfs</a> *);
 <a name="L34" href="source/fs/qnx4/inode.c#L34">34</a> 
 <a name="L35" href="source/fs/qnx4/inode.c#L35">35</a> static const struct <a href="ident?i=super_operations">super_operations</a> <a href="ident?i=qnx4_sops">qnx4_sops</a> =
 <a name="L36" href="source/fs/qnx4/inode.c#L36">36</a> {
 <a name="L37" href="source/fs/qnx4/inode.c#L37">37</a>         .<a href="ident?i=alloc_inode">alloc_inode</a>    = <a href="ident?i=qnx4_alloc_inode">qnx4_alloc_inode</a>,
 <a name="L38" href="source/fs/qnx4/inode.c#L38">38</a>         .<a href="ident?i=destroy_inode">destroy_inode</a>  = <a href="ident?i=qnx4_destroy_inode">qnx4_destroy_inode</a>,
 <a name="L39" href="source/fs/qnx4/inode.c#L39">39</a>         .<a href="ident?i=statfs">statfs</a>         = <a href="ident?i=qnx4_statfs">qnx4_statfs</a>,
 <a name="L40" href="source/fs/qnx4/inode.c#L40">40</a>         .remount_fs     = <a href="ident?i=qnx4_remount">qnx4_remount</a>,
 <a name="L41" href="source/fs/qnx4/inode.c#L41">41</a> };
 <a name="L42" href="source/fs/qnx4/inode.c#L42">42</a> 
 <a name="L43" href="source/fs/qnx4/inode.c#L43">43</a> static int <a href="ident?i=qnx4_remount">qnx4_remount</a>(struct <a href="ident?i=super_block">super_block</a> *<a href="ident?i=sb">sb</a>, int *<a href="ident?i=flags">flags</a>, char *<a href="ident?i=data">data</a>)
 <a name="L44" href="source/fs/qnx4/inode.c#L44">44</a> {
 <a name="L45" href="source/fs/qnx4/inode.c#L45">45</a>         struct <a href="ident?i=qnx4_sb_info">qnx4_sb_info</a> *qs;
 <a name="L46" href="source/fs/qnx4/inode.c#L46">46</a> 
 <a name="L47" href="source/fs/qnx4/inode.c#L47">47</a>         qs = <a href="ident?i=qnx4_sb">qnx4_sb</a>(<a href="ident?i=sb">sb</a>);
 <a name="L48" href="source/fs/qnx4/inode.c#L48">48</a>         qs-&gt;Version = <a href="ident?i=QNX4_VERSION">QNX4_VERSION</a>;
 <a name="L49" href="source/fs/qnx4/inode.c#L49">49</a>         *<a href="ident?i=flags">flags</a> |= <a href="ident?i=MS_RDONLY">MS_RDONLY</a>;
 <a name="L50" href="source/fs/qnx4/inode.c#L50">50</a>         return 0;
 <a name="L51" href="source/fs/qnx4/inode.c#L51">51</a> }
 <a name="L52" href="source/fs/qnx4/inode.c#L52">52</a> 
 <a name="L53" href="source/fs/qnx4/inode.c#L53">53</a> static int <a href="ident?i=qnx4_get_block">qnx4_get_block</a>( struct <a href="ident?i=inode">inode</a> *<a href="ident?i=inode">inode</a>, <a href="ident?i=sector_t">sector_t</a> iblock, struct <a href="ident?i=buffer_head">buffer_head</a> *<a href="ident?i=bh">bh</a>, int <a href="ident?i=create">create</a> )
 <a name="L54" href="source/fs/qnx4/inode.c#L54">54</a> {
 <a name="L55" href="source/fs/qnx4/inode.c#L55">55</a>         unsigned long phys;
 <a name="L56" href="source/fs/qnx4/inode.c#L56">56</a> 
 <a name="L57" href="source/fs/qnx4/inode.c#L57">57</a>         <a href="ident?i=QNX4DEBUG">QNX4DEBUG</a>((<a href="ident?i=KERN_INFO">KERN_INFO</a> <i>"qnx4: qnx4_get_block inode=[%ld] iblock=[%ld]\n"</i>,<a href="ident?i=inode">inode</a>-&gt;i_ino,iblock));
 <a name="L58" href="source/fs/qnx4/inode.c#L58">58</a> 
 <a name="L59" href="source/fs/qnx4/inode.c#L59">59</a>         phys = <a href="ident?i=qnx4_block_map">qnx4_block_map</a>( <a href="ident?i=inode">inode</a>, iblock );
 <a name="L60" href="source/fs/qnx4/inode.c#L60">60</a>         if ( phys ) {
 <a name="L61" href="source/fs/qnx4/inode.c#L61">61</a>                 <b><i>// logical block is before EOF</i></b>
 <a name="L62" href="source/fs/qnx4/inode.c#L62">62</a> <b><i></i></b>                <a href="ident?i=map_bh">map_bh</a>(<a href="ident?i=bh">bh</a>, <a href="ident?i=inode">inode</a>-&gt;i_sb, phys);
 <a name="L63" href="source/fs/qnx4/inode.c#L63">63</a>         }
 <a name="L64" href="source/fs/qnx4/inode.c#L64">64</a>         return 0;
 <a name="L65" href="source/fs/qnx4/inode.c#L65">65</a> }
 <a name="L66" href="source/fs/qnx4/inode.c#L66">66</a> 
 <a name="L67" href="source/fs/qnx4/inode.c#L67">67</a> static inline <a href="ident?i=u32">u32</a> <a href="ident?i=try_extent">try_extent</a>(<a href="ident?i=qnx4_xtnt_t">qnx4_xtnt_t</a> *extent, <a href="ident?i=u32">u32</a> *<a href="ident?i=offset">offset</a>)
 <a name="L68" href="source/fs/qnx4/inode.c#L68">68</a> {
 <a name="L69" href="source/fs/qnx4/inode.c#L69">69</a>         <a href="ident?i=u32">u32</a> <a href="ident?i=size">size</a> = <a href="ident?i=le32_to_cpu">le32_to_cpu</a>(extent-&gt;xtnt_size);
 <a name="L70" href="source/fs/qnx4/inode.c#L70">70</a>         if (*<a href="ident?i=offset">offset</a> &lt; <a href="ident?i=size">size</a>)
 <a name="L71" href="source/fs/qnx4/inode.c#L71">71</a>                 return <a href="ident?i=le32_to_cpu">le32_to_cpu</a>(extent-&gt;xtnt_blk) + *<a href="ident?i=offset">offset</a> - 1;
 <a name="L72" href="source/fs/qnx4/inode.c#L72">72</a>         *<a href="ident?i=offset">offset</a> -= <a href="ident?i=size">size</a>;
 <a name="L73" href="source/fs/qnx4/inode.c#L73">73</a>         return 0;
 <a name="L74" href="source/fs/qnx4/inode.c#L74">74</a> }
 <a name="L75" href="source/fs/qnx4/inode.c#L75">75</a> 
 <a name="L76" href="source/fs/qnx4/inode.c#L76">76</a> unsigned long <a href="ident?i=qnx4_block_map">qnx4_block_map</a>( struct <a href="ident?i=inode">inode</a> *<a href="ident?i=inode">inode</a>, long iblock )
 <a name="L77" href="source/fs/qnx4/inode.c#L77">77</a> {
 <a name="L78" href="source/fs/qnx4/inode.c#L78">78</a>         int ix;
 <a name="L79" href="source/fs/qnx4/inode.c#L79">79</a>         long i_xblk;
 <a name="L80" href="source/fs/qnx4/inode.c#L80">80</a>         struct <a href="ident?i=buffer_head">buffer_head</a> *<a href="ident?i=bh">bh</a> = <a href="ident?i=NULL">NULL</a>;
 <a name="L81" href="source/fs/qnx4/inode.c#L81">81</a>         struct <a href="ident?i=qnx4_xblk">qnx4_xblk</a> *xblk = <a href="ident?i=NULL">NULL</a>;
 <a name="L82" href="source/fs/qnx4/inode.c#L82">82</a>         struct <a href="ident?i=qnx4_inode_entry">qnx4_inode_entry</a> *qnx4_inode = <a href="ident?i=qnx4_raw_inode">qnx4_raw_inode</a>(<a href="ident?i=inode">inode</a>);
 <a name="L83" href="source/fs/qnx4/inode.c#L83">83</a>         <a href="ident?i=u16">u16</a> nxtnt = <a href="ident?i=le16_to_cpu">le16_to_cpu</a>(qnx4_inode-&gt;di_num_xtnts);
 <a name="L84" href="source/fs/qnx4/inode.c#L84">84</a>         <a href="ident?i=u32">u32</a> <a href="ident?i=offset">offset</a> = iblock;
 <a name="L85" href="source/fs/qnx4/inode.c#L85">85</a>         <a href="ident?i=u32">u32</a> <a href="ident?i=block">block</a> = <a href="ident?i=try_extent">try_extent</a>(&amp;qnx4_inode-&gt;di_first_xtnt, &amp;<a href="ident?i=offset">offset</a>);
 <a name="L86" href="source/fs/qnx4/inode.c#L86">86</a> 
 <a name="L87" href="source/fs/qnx4/inode.c#L87">87</a>         if (<a href="ident?i=block">block</a>) {
 <a name="L88" href="source/fs/qnx4/inode.c#L88">88</a>                 <b><i>// iblock is in the first extent. This is easy.</i></b>
 <a name="L89" href="source/fs/qnx4/inode.c#L89">89</a> <b><i></i></b>        } else {
 <a name="L90" href="source/fs/qnx4/inode.c#L90">90</a>                 <b><i>// iblock is beyond first extent. We have to follow the extent chain.</i></b>
 <a name="L91" href="source/fs/qnx4/inode.c#L91">91</a> <b><i></i></b>                i_xblk = <a href="ident?i=le32_to_cpu">le32_to_cpu</a>(qnx4_inode-&gt;di_xblk);
 <a name="L92" href="source/fs/qnx4/inode.c#L92">92</a>                 ix = 0;
 <a name="L93" href="source/fs/qnx4/inode.c#L93">93</a>                 while ( --nxtnt &gt; 0 ) {
 <a name="L94" href="source/fs/qnx4/inode.c#L94">94</a>                         if ( ix == 0 ) {
 <a name="L95" href="source/fs/qnx4/inode.c#L95">95</a>                                 <b><i>// read next xtnt block.</i></b>
 <a name="L96" href="source/fs/qnx4/inode.c#L96">96</a> <b><i></i></b>                                <a href="ident?i=bh">bh</a> = <a href="ident?i=sb_bread">sb_bread</a>(<a href="ident?i=inode">inode</a>-&gt;i_sb, i_xblk - 1);
 <a name="L97" href="source/fs/qnx4/inode.c#L97">97</a>                                 if ( !<a href="ident?i=bh">bh</a> ) {
 <a name="L98" href="source/fs/qnx4/inode.c#L98">98</a>                                         <a href="ident?i=QNX4DEBUG">QNX4DEBUG</a>((<a href="ident?i=KERN_ERR">KERN_ERR</a> <i>"qnx4: I/O error reading xtnt block [%ld])\n"</i>, i_xblk - 1));
 <a name="L99" href="source/fs/qnx4/inode.c#L99">99</a>                                         return -<a href="ident?i=EIO">EIO</a>;
<a name="L100" href="source/fs/qnx4/inode.c#L100">100</a>                                 }
<a name="L101" href="source/fs/qnx4/inode.c#L101">101</a>                                 xblk = (struct <a href="ident?i=qnx4_xblk">qnx4_xblk</a>*)<a href="ident?i=bh">bh</a>-&gt;b_data;
<a name="L102" href="source/fs/qnx4/inode.c#L102">102</a>                                 if ( <a href="ident?i=memcmp">memcmp</a>( xblk-&gt;xblk_signature, <i>"IamXblk"</i>, 7 ) ) {
<a name="L103" href="source/fs/qnx4/inode.c#L103">103</a>                                         <a href="ident?i=QNX4DEBUG">QNX4DEBUG</a>((<a href="ident?i=KERN_ERR">KERN_ERR</a> <i>"qnx4: block at %ld is not a valid xtnt\n"</i>, qnx4_inode-&gt;i_xblk));
<a name="L104" href="source/fs/qnx4/inode.c#L104">104</a>                                         return -<a href="ident?i=EIO">EIO</a>;
<a name="L105" href="source/fs/qnx4/inode.c#L105">105</a>                                 }
<a name="L106" href="source/fs/qnx4/inode.c#L106">106</a>                         }
<a name="L107" href="source/fs/qnx4/inode.c#L107">107</a>                         <a href="ident?i=block">block</a> = <a href="ident?i=try_extent">try_extent</a>(&amp;xblk-&gt;xblk_xtnts[ix], &amp;<a href="ident?i=offset">offset</a>);
<a name="L108" href="source/fs/qnx4/inode.c#L108">108</a>                         if (<a href="ident?i=block">block</a>) {
<a name="L109" href="source/fs/qnx4/inode.c#L109">109</a>                                 <b><i>// got it!</i></b>
<a name="L110" href="source/fs/qnx4/inode.c#L110">110</a> <b><i></i></b>                                break;
<a name="L111" href="source/fs/qnx4/inode.c#L111">111</a>                         }
<a name="L112" href="source/fs/qnx4/inode.c#L112">112</a>                         if ( ++ix &gt;= xblk-&gt;xblk_num_xtnts ) {
<a name="L113" href="source/fs/qnx4/inode.c#L113">113</a>                                 i_xblk = <a href="ident?i=le32_to_cpu">le32_to_cpu</a>(xblk-&gt;xblk_next_xblk);
<a name="L114" href="source/fs/qnx4/inode.c#L114">114</a>                                 ix = 0;
<a name="L115" href="source/fs/qnx4/inode.c#L115">115</a>                                 <a href="ident?i=brelse">brelse</a>( <a href="ident?i=bh">bh</a> );
<a name="L116" href="source/fs/qnx4/inode.c#L116">116</a>                                 <a href="ident?i=bh">bh</a> = <a href="ident?i=NULL">NULL</a>;
<a name="L117" href="source/fs/qnx4/inode.c#L117">117</a>                         }
<a name="L118" href="source/fs/qnx4/inode.c#L118">118</a>                 }
<a name="L119" href="source/fs/qnx4/inode.c#L119">119</a>                 if ( <a href="ident?i=bh">bh</a> )
<a name="L120" href="source/fs/qnx4/inode.c#L120">120</a>                         <a href="ident?i=brelse">brelse</a>( <a href="ident?i=bh">bh</a> );
<a name="L121" href="source/fs/qnx4/inode.c#L121">121</a>         }
<a name="L122" href="source/fs/qnx4/inode.c#L122">122</a> 
<a name="L123" href="source/fs/qnx4/inode.c#L123">123</a>         <a href="ident?i=QNX4DEBUG">QNX4DEBUG</a>((<a href="ident?i=KERN_INFO">KERN_INFO</a> <i>"qnx4: mapping block %ld of inode %ld = %ld\n"</i>,iblock,<a href="ident?i=inode">inode</a>-&gt;i_ino,<a href="ident?i=block">block</a>));
<a name="L124" href="source/fs/qnx4/inode.c#L124">124</a>         return <a href="ident?i=block">block</a>;
<a name="L125" href="source/fs/qnx4/inode.c#L125">125</a> }
<a name="L126" href="source/fs/qnx4/inode.c#L126">126</a> 
<a name="L127" href="source/fs/qnx4/inode.c#L127">127</a> static int <a href="ident?i=qnx4_statfs">qnx4_statfs</a>(struct <a href="ident?i=dentry">dentry</a> *<a href="ident?i=dentry">dentry</a>, struct <a href="ident?i=kstatfs">kstatfs</a> *<a href="ident?i=buf">buf</a>)
<a name="L128" href="source/fs/qnx4/inode.c#L128">128</a> {
<a name="L129" href="source/fs/qnx4/inode.c#L129">129</a>         struct <a href="ident?i=super_block">super_block</a> *<a href="ident?i=sb">sb</a> = <a href="ident?i=dentry">dentry</a>-&gt;d_sb;
<a name="L130" href="source/fs/qnx4/inode.c#L130">130</a>         <a href="ident?i=u64">u64</a> <a href="ident?i=id">id</a> = <a href="ident?i=huge_encode_dev">huge_encode_dev</a>(<a href="ident?i=sb">sb</a>-&gt;s_bdev-&gt;bd_dev);
<a name="L131" href="source/fs/qnx4/inode.c#L131">131</a> 
<a name="L132" href="source/fs/qnx4/inode.c#L132">132</a>         <a href="ident?i=buf">buf</a>-&gt;f_type    = <a href="ident?i=sb">sb</a>-&gt;s_magic;
<a name="L133" href="source/fs/qnx4/inode.c#L133">133</a>         <a href="ident?i=buf">buf</a>-&gt;f_bsize   = <a href="ident?i=sb">sb</a>-&gt;s_blocksize;
<a name="L134" href="source/fs/qnx4/inode.c#L134">134</a>         <a href="ident?i=buf">buf</a>-&gt;f_blocks  = <a href="ident?i=le32_to_cpu">le32_to_cpu</a>(<a href="ident?i=qnx4_sb">qnx4_sb</a>(<a href="ident?i=sb">sb</a>)-&gt;BitMap-&gt;di_size) * 8;
<a name="L135" href="source/fs/qnx4/inode.c#L135">135</a>         <a href="ident?i=buf">buf</a>-&gt;f_bfree   = <a href="ident?i=qnx4_count_free_blocks">qnx4_count_free_blocks</a>(<a href="ident?i=sb">sb</a>);
<a name="L136" href="source/fs/qnx4/inode.c#L136">136</a>         <a href="ident?i=buf">buf</a>-&gt;f_bavail  = <a href="ident?i=buf">buf</a>-&gt;f_bfree;
<a name="L137" href="source/fs/qnx4/inode.c#L137">137</a>         <a href="ident?i=buf">buf</a>-&gt;f_namelen = <a href="ident?i=QNX4_NAME_MAX">QNX4_NAME_MAX</a>;
<a name="L138" href="source/fs/qnx4/inode.c#L138">138</a>         <a href="ident?i=buf">buf</a>-&gt;f_fsid.<a href="ident?i=val">val</a>[0] = (<a href="ident?i=u32">u32</a>)<a href="ident?i=id">id</a>;
<a name="L139" href="source/fs/qnx4/inode.c#L139">139</a>         <a href="ident?i=buf">buf</a>-&gt;f_fsid.<a href="ident?i=val">val</a>[1] = (<a href="ident?i=u32">u32</a>)(<a href="ident?i=id">id</a> &gt;&gt; 32);
<a name="L140" href="source/fs/qnx4/inode.c#L140">140</a> 
<a name="L141" href="source/fs/qnx4/inode.c#L141">141</a>         return 0;
<a name="L142" href="source/fs/qnx4/inode.c#L142">142</a> }
<a name="L143" href="source/fs/qnx4/inode.c#L143">143</a> 
<a name="L144" href="source/fs/qnx4/inode.c#L144">144</a> <b><i>/*</i></b>
<a name="L145" href="source/fs/qnx4/inode.c#L145">145</a> <b><i> * Check the root directory of the filesystem to make sure</i></b>
<a name="L146" href="source/fs/qnx4/inode.c#L146">146</a> <b><i> * it really _is_ a qnx4 filesystem, and to check the size</i></b>
<a name="L147" href="source/fs/qnx4/inode.c#L147">147</a> <b><i> * of the directory entry.</i></b>
<a name="L148" href="source/fs/qnx4/inode.c#L148">148</a> <b><i> */</i></b>
<a name="L149" href="source/fs/qnx4/inode.c#L149">149</a> static const char *<a href="ident?i=qnx4_checkroot">qnx4_checkroot</a>(struct <a href="ident?i=super_block">super_block</a> *<a href="ident?i=sb">sb</a>,
<a name="L150" href="source/fs/qnx4/inode.c#L150">150</a>                                   struct <a href="ident?i=qnx4_super_block">qnx4_super_block</a> *<a href="ident?i=s">s</a>)
<a name="L151" href="source/fs/qnx4/inode.c#L151">151</a> {
<a name="L152" href="source/fs/qnx4/inode.c#L152">152</a>         struct <a href="ident?i=buffer_head">buffer_head</a> *<a href="ident?i=bh">bh</a>;
<a name="L153" href="source/fs/qnx4/inode.c#L153">153</a>         struct <a href="ident?i=qnx4_inode_entry">qnx4_inode_entry</a> *<a href="ident?i=rootdir">rootdir</a>;
<a name="L154" href="source/fs/qnx4/inode.c#L154">154</a>         int <a href="ident?i=rd">rd</a>, rl;
<a name="L155" href="source/fs/qnx4/inode.c#L155">155</a>         int <a href="ident?i=i">i</a>, j;
<a name="L156" href="source/fs/qnx4/inode.c#L156">156</a> 
<a name="L157" href="source/fs/qnx4/inode.c#L157">157</a>         if (<a href="ident?i=s">s</a>-&gt;RootDir.di_fname[0] != <i>'/'</i> || <a href="ident?i=s">s</a>-&gt;RootDir.di_fname[1] != <i>'\0'</i>)
<a name="L158" href="source/fs/qnx4/inode.c#L158">158</a>                 return <i>"no qnx4 filesystem (no root dir)."</i>;
<a name="L159" href="source/fs/qnx4/inode.c#L159">159</a>         <a href="ident?i=QNX4DEBUG">QNX4DEBUG</a>((<a href="ident?i=KERN_NOTICE">KERN_NOTICE</a> <i>"QNX4 filesystem found on dev %s.\n"</i>, <a href="ident?i=sb">sb</a>-&gt;s_id));
<a name="L160" href="source/fs/qnx4/inode.c#L160">160</a>         <a href="ident?i=rd">rd</a> = <a href="ident?i=le32_to_cpu">le32_to_cpu</a>(<a href="ident?i=s">s</a>-&gt;RootDir.di_first_xtnt.xtnt_blk) - 1;
<a name="L161" href="source/fs/qnx4/inode.c#L161">161</a>         rl = <a href="ident?i=le32_to_cpu">le32_to_cpu</a>(<a href="ident?i=s">s</a>-&gt;RootDir.di_first_xtnt.xtnt_size);
<a name="L162" href="source/fs/qnx4/inode.c#L162">162</a>         for (j = 0; j &lt; rl; j++) {
<a name="L163" href="source/fs/qnx4/inode.c#L163">163</a>                 <a href="ident?i=bh">bh</a> = <a href="ident?i=sb_bread">sb_bread</a>(<a href="ident?i=sb">sb</a>, <a href="ident?i=rd">rd</a> + j);      <b><i>/* root dir, first block */</i></b>
<a name="L164" href="source/fs/qnx4/inode.c#L164">164</a>                 if (<a href="ident?i=bh">bh</a> == <a href="ident?i=NULL">NULL</a>)
<a name="L165" href="source/fs/qnx4/inode.c#L165">165</a>                         return <i>"unable to read root entry."</i>;
<a name="L166" href="source/fs/qnx4/inode.c#L166">166</a>                 <a href="ident?i=rootdir">rootdir</a> = (struct <a href="ident?i=qnx4_inode_entry">qnx4_inode_entry</a> *) <a href="ident?i=bh">bh</a>-&gt;b_data;
<a name="L167" href="source/fs/qnx4/inode.c#L167">167</a>                 for (<a href="ident?i=i">i</a> = 0; <a href="ident?i=i">i</a> &lt; <a href="ident?i=QNX4_INODES_PER_BLOCK">QNX4_INODES_PER_BLOCK</a>; <a href="ident?i=i">i</a>++, <a href="ident?i=rootdir">rootdir</a>++) {
<a name="L168" href="source/fs/qnx4/inode.c#L168">168</a>                         <a href="ident?i=QNX4DEBUG">QNX4DEBUG</a>((<a href="ident?i=KERN_INFO">KERN_INFO</a> <i>"rootdir entry found : [%s]\n"</i>, <a href="ident?i=rootdir">rootdir</a>-&gt;di_fname));
<a name="L169" href="source/fs/qnx4/inode.c#L169">169</a>                         if (<a href="ident?i=strcmp">strcmp</a>(<a href="ident?i=rootdir">rootdir</a>-&gt;di_fname, <a href="ident?i=QNX4_BMNAME">QNX4_BMNAME</a>) != 0)
<a name="L170" href="source/fs/qnx4/inode.c#L170">170</a>                                 continue;
<a name="L171" href="source/fs/qnx4/inode.c#L171">171</a>                         <a href="ident?i=qnx4_sb">qnx4_sb</a>(<a href="ident?i=sb">sb</a>)-&gt;BitMap = <a href="ident?i=kmemdup">kmemdup</a>(<a href="ident?i=rootdir">rootdir</a>,
<a name="L172" href="source/fs/qnx4/inode.c#L172">172</a>                                                       sizeof(struct <a href="ident?i=qnx4_inode_entry">qnx4_inode_entry</a>),
<a name="L173" href="source/fs/qnx4/inode.c#L173">173</a>                                                       <a href="ident?i=GFP_KERNEL">GFP_KERNEL</a>);
<a name="L174" href="source/fs/qnx4/inode.c#L174">174</a>                         <a href="ident?i=brelse">brelse</a>(<a href="ident?i=bh">bh</a>);
<a name="L175" href="source/fs/qnx4/inode.c#L175">175</a>                         if (!<a href="ident?i=qnx4_sb">qnx4_sb</a>(<a href="ident?i=sb">sb</a>)-&gt;BitMap)
<a name="L176" href="source/fs/qnx4/inode.c#L176">176</a>                                 return <i>"not enough memory for bitmap inode"</i>;
<a name="L177" href="source/fs/qnx4/inode.c#L177">177</a>                         <b><i>/* keep bitmap inode known */</i></b>
<a name="L178" href="source/fs/qnx4/inode.c#L178">178</a>                         return <a href="ident?i=NULL">NULL</a>;
<a name="L179" href="source/fs/qnx4/inode.c#L179">179</a>                 }
<a name="L180" href="source/fs/qnx4/inode.c#L180">180</a>                 <a href="ident?i=brelse">brelse</a>(<a href="ident?i=bh">bh</a>);
<a name="L181" href="source/fs/qnx4/inode.c#L181">181</a>         }
<a name="L182" href="source/fs/qnx4/inode.c#L182">182</a>         return <i>"bitmap file not found."</i>;
<a name="L183" href="source/fs/qnx4/inode.c#L183">183</a> }
<a name="L184" href="source/fs/qnx4/inode.c#L184">184</a> 
<a name="L185" href="source/fs/qnx4/inode.c#L185">185</a> static int <a href="ident?i=qnx4_fill_super">qnx4_fill_super</a>(struct <a href="ident?i=super_block">super_block</a> *<a href="ident?i=s">s</a>, void *<a href="ident?i=data">data</a>, int silent)
<a name="L186" href="source/fs/qnx4/inode.c#L186">186</a> {
<a name="L187" href="source/fs/qnx4/inode.c#L187">187</a>         struct <a href="ident?i=buffer_head">buffer_head</a> *<a href="ident?i=bh">bh</a>;
<a name="L188" href="source/fs/qnx4/inode.c#L188">188</a>         struct <a href="ident?i=inode">inode</a> *<a href="ident?i=root">root</a>;
<a name="L189" href="source/fs/qnx4/inode.c#L189">189</a>         const char *errmsg;
<a name="L190" href="source/fs/qnx4/inode.c#L190">190</a>         struct <a href="ident?i=qnx4_sb_info">qnx4_sb_info</a> *qs;
<a name="L191" href="source/fs/qnx4/inode.c#L191">191</a> 
<a name="L192" href="source/fs/qnx4/inode.c#L192">192</a>         qs = <a href="ident?i=kzalloc">kzalloc</a>(sizeof(struct <a href="ident?i=qnx4_sb_info">qnx4_sb_info</a>), <a href="ident?i=GFP_KERNEL">GFP_KERNEL</a>);
<a name="L193" href="source/fs/qnx4/inode.c#L193">193</a>         if (!qs)
<a name="L194" href="source/fs/qnx4/inode.c#L194">194</a>                 return -<a href="ident?i=ENOMEM">ENOMEM</a>;
<a name="L195" href="source/fs/qnx4/inode.c#L195">195</a>         <a href="ident?i=s">s</a>-&gt;s_fs_info = qs;
<a name="L196" href="source/fs/qnx4/inode.c#L196">196</a> 
<a name="L197" href="source/fs/qnx4/inode.c#L197">197</a>         <a href="ident?i=sb_set_blocksize">sb_set_blocksize</a>(<a href="ident?i=s">s</a>, <a href="ident?i=QNX4_BLOCK_SIZE">QNX4_BLOCK_SIZE</a>);
<a name="L198" href="source/fs/qnx4/inode.c#L198">198</a> 
<a name="L199" href="source/fs/qnx4/inode.c#L199">199</a>         <a href="ident?i=s">s</a>-&gt;s_op = &amp;<a href="ident?i=qnx4_sops">qnx4_sops</a>;
<a name="L200" href="source/fs/qnx4/inode.c#L200">200</a>         <a href="ident?i=s">s</a>-&gt;s_magic = <a href="ident?i=QNX4_SUPER_MAGIC">QNX4_SUPER_MAGIC</a>;
<a name="L201" href="source/fs/qnx4/inode.c#L201">201</a>         <a href="ident?i=s">s</a>-&gt;s_flags |= <a href="ident?i=MS_RDONLY">MS_RDONLY</a>;        <b><i>/* Yup, read-only yet */</i></b>
<a name="L202" href="source/fs/qnx4/inode.c#L202">202</a> 
<a name="L203" href="source/fs/qnx4/inode.c#L203">203</a>         <b><i>/* Check the superblock signature. Since the qnx4 code is</i></b>
<a name="L204" href="source/fs/qnx4/inode.c#L204">204</a> <b><i>           dangerous, we should leave as quickly as possible</i></b>
<a name="L205" href="source/fs/qnx4/inode.c#L205">205</a> <b><i>           if we don't belong here... */</i></b>
<a name="L206" href="source/fs/qnx4/inode.c#L206">206</a>         <a href="ident?i=bh">bh</a> = <a href="ident?i=sb_bread">sb_bread</a>(<a href="ident?i=s">s</a>, 1);
<a name="L207" href="source/fs/qnx4/inode.c#L207">207</a>         if (!<a href="ident?i=bh">bh</a>) {
<a name="L208" href="source/fs/qnx4/inode.c#L208">208</a>                 <a href="ident?i=printk">printk</a>(<a href="ident?i=KERN_ERR">KERN_ERR</a> <i>"qnx4: unable to read the superblock\n"</i>);
<a name="L209" href="source/fs/qnx4/inode.c#L209">209</a>                 return -<a href="ident?i=EINVAL">EINVAL</a>;
<a name="L210" href="source/fs/qnx4/inode.c#L210">210</a>         }
<a name="L211" href="source/fs/qnx4/inode.c#L211">211</a> 
<a name="L212" href="source/fs/qnx4/inode.c#L212">212</a>         <b><i>/* check before allocating dentries, inodes, .. */</i></b>
<a name="L213" href="source/fs/qnx4/inode.c#L213">213</a>         errmsg = <a href="ident?i=qnx4_checkroot">qnx4_checkroot</a>(<a href="ident?i=s">s</a>, (struct <a href="ident?i=qnx4_super_block">qnx4_super_block</a> *) <a href="ident?i=bh">bh</a>-&gt;b_data);
<a name="L214" href="source/fs/qnx4/inode.c#L214">214</a>         <a href="ident?i=brelse">brelse</a>(<a href="ident?i=bh">bh</a>);
<a name="L215" href="source/fs/qnx4/inode.c#L215">215</a>         if (errmsg != <a href="ident?i=NULL">NULL</a>) {
<a name="L216" href="source/fs/qnx4/inode.c#L216">216</a>                 if (!silent)
<a name="L217" href="source/fs/qnx4/inode.c#L217">217</a>                         <a href="ident?i=printk">printk</a>(<a href="ident?i=KERN_ERR">KERN_ERR</a> <i>"qnx4: %s\n"</i>, errmsg);
<a name="L218" href="source/fs/qnx4/inode.c#L218">218</a>                 return -<a href="ident?i=EINVAL">EINVAL</a>;
<a name="L219" href="source/fs/qnx4/inode.c#L219">219</a>         }
<a name="L220" href="source/fs/qnx4/inode.c#L220">220</a> 
<a name="L221" href="source/fs/qnx4/inode.c#L221">221</a>         <b><i>/* does root not have inode number QNX4_ROOT_INO ?? */</i></b>
<a name="L222" href="source/fs/qnx4/inode.c#L222">222</a>         <a href="ident?i=root">root</a> = <a href="ident?i=qnx4_iget">qnx4_iget</a>(<a href="ident?i=s">s</a>, <a href="ident?i=QNX4_ROOT_INO">QNX4_ROOT_INO</a> * <a href="ident?i=QNX4_INODES_PER_BLOCK">QNX4_INODES_PER_BLOCK</a>);
<a name="L223" href="source/fs/qnx4/inode.c#L223">223</a>         if (<a href="ident?i=IS_ERR">IS_ERR</a>(<a href="ident?i=root">root</a>)) {
<a name="L224" href="source/fs/qnx4/inode.c#L224">224</a>                 <a href="ident?i=printk">printk</a>(<a href="ident?i=KERN_ERR">KERN_ERR</a> <i>"qnx4: get inode failed\n"</i>);
<a name="L225" href="source/fs/qnx4/inode.c#L225">225</a>                 return <a href="ident?i=PTR_ERR">PTR_ERR</a>(<a href="ident?i=root">root</a>);
<a name="L226" href="source/fs/qnx4/inode.c#L226">226</a>         }
<a name="L227" href="source/fs/qnx4/inode.c#L227">227</a> 
<a name="L228" href="source/fs/qnx4/inode.c#L228">228</a>         <a href="ident?i=s">s</a>-&gt;s_root = <a href="ident?i=d_make_root">d_make_root</a>(<a href="ident?i=root">root</a>);
<a name="L229" href="source/fs/qnx4/inode.c#L229">229</a>         if (<a href="ident?i=s">s</a>-&gt;s_root == <a href="ident?i=NULL">NULL</a>)
<a name="L230" href="source/fs/qnx4/inode.c#L230">230</a>                 return -<a href="ident?i=ENOMEM">ENOMEM</a>;
<a name="L231" href="source/fs/qnx4/inode.c#L231">231</a> 
<a name="L232" href="source/fs/qnx4/inode.c#L232">232</a>         return 0;
<a name="L233" href="source/fs/qnx4/inode.c#L233">233</a> }
<a name="L234" href="source/fs/qnx4/inode.c#L234">234</a> 
<a name="L235" href="source/fs/qnx4/inode.c#L235">235</a> static void <a href="ident?i=qnx4_kill_sb">qnx4_kill_sb</a>(struct <a href="ident?i=super_block">super_block</a> *<a href="ident?i=sb">sb</a>)
<a name="L236" href="source/fs/qnx4/inode.c#L236">236</a> {
<a name="L237" href="source/fs/qnx4/inode.c#L237">237</a>         struct <a href="ident?i=qnx4_sb_info">qnx4_sb_info</a> *qs = <a href="ident?i=qnx4_sb">qnx4_sb</a>(<a href="ident?i=sb">sb</a>);
<a name="L238" href="source/fs/qnx4/inode.c#L238">238</a>         <a href="ident?i=kill_block_super">kill_block_super</a>(<a href="ident?i=sb">sb</a>);
<a name="L239" href="source/fs/qnx4/inode.c#L239">239</a>         if (qs) {
<a name="L240" href="source/fs/qnx4/inode.c#L240">240</a>                 <a href="ident?i=kfree">kfree</a>(qs-&gt;BitMap);
<a name="L241" href="source/fs/qnx4/inode.c#L241">241</a>                 <a href="ident?i=kfree">kfree</a>(qs);
<a name="L242" href="source/fs/qnx4/inode.c#L242">242</a>         }
<a name="L243" href="source/fs/qnx4/inode.c#L243">243</a> }
<a name="L244" href="source/fs/qnx4/inode.c#L244">244</a> 
<a name="L245" href="source/fs/qnx4/inode.c#L245">245</a> static int <a href="ident?i=qnx4_readpage">qnx4_readpage</a>(struct <a href="ident?i=file">file</a> *<a href="ident?i=file">file</a>, struct <a href="ident?i=page">page</a> *<a href="ident?i=page">page</a>)
<a name="L246" href="source/fs/qnx4/inode.c#L246">246</a> {
<a name="L247" href="source/fs/qnx4/inode.c#L247">247</a>         return <a href="ident?i=block_read_full_page">block_read_full_page</a>(<a href="ident?i=page">page</a>,<a href="ident?i=qnx4_get_block">qnx4_get_block</a>);
<a name="L248" href="source/fs/qnx4/inode.c#L248">248</a> }
<a name="L249" href="source/fs/qnx4/inode.c#L249">249</a> 
<a name="L250" href="source/fs/qnx4/inode.c#L250">250</a> static <a href="ident?i=sector_t">sector_t</a> <a href="ident?i=qnx4_bmap">qnx4_bmap</a>(struct <a href="ident?i=address_space">address_space</a> *mapping, <a href="ident?i=sector_t">sector_t</a> <a href="ident?i=block">block</a>)
<a name="L251" href="source/fs/qnx4/inode.c#L251">251</a> {
<a name="L252" href="source/fs/qnx4/inode.c#L252">252</a>         return <a href="ident?i=generic_block_bmap">generic_block_bmap</a>(mapping,<a href="ident?i=block">block</a>,<a href="ident?i=qnx4_get_block">qnx4_get_block</a>);
<a name="L253" href="source/fs/qnx4/inode.c#L253">253</a> }
<a name="L254" href="source/fs/qnx4/inode.c#L254">254</a> static const struct <a href="ident?i=address_space_operations">address_space_operations</a> <a href="ident?i=qnx4_aops">qnx4_aops</a> = {
<a name="L255" href="source/fs/qnx4/inode.c#L255">255</a>         .readpage       = <a href="ident?i=qnx4_readpage">qnx4_readpage</a>,
<a name="L256" href="source/fs/qnx4/inode.c#L256">256</a>         .<a href="ident?i=bmap">bmap</a>           = <a href="ident?i=qnx4_bmap">qnx4_bmap</a>
<a name="L257" href="source/fs/qnx4/inode.c#L257">257</a> };
<a name="L258" href="source/fs/qnx4/inode.c#L258">258</a> 
<a name="L259" href="source/fs/qnx4/inode.c#L259">259</a> struct <a href="ident?i=inode">inode</a> *<a href="ident?i=qnx4_iget">qnx4_iget</a>(struct <a href="ident?i=super_block">super_block</a> *<a href="ident?i=sb">sb</a>, unsigned long <a href="ident?i=ino">ino</a>)
<a name="L260" href="source/fs/qnx4/inode.c#L260">260</a> {
<a name="L261" href="source/fs/qnx4/inode.c#L261">261</a>         struct <a href="ident?i=buffer_head">buffer_head</a> *<a href="ident?i=bh">bh</a>;
<a name="L262" href="source/fs/qnx4/inode.c#L262">262</a>         struct <a href="ident?i=qnx4_inode_entry">qnx4_inode_entry</a> *raw_inode;
<a name="L263" href="source/fs/qnx4/inode.c#L263">263</a>         int <a href="ident?i=block">block</a>;
<a name="L264" href="source/fs/qnx4/inode.c#L264">264</a>         struct <a href="ident?i=qnx4_inode_entry">qnx4_inode_entry</a> *qnx4_inode;
<a name="L265" href="source/fs/qnx4/inode.c#L265">265</a>         struct <a href="ident?i=inode">inode</a> *<a href="ident?i=inode">inode</a>;
<a name="L266" href="source/fs/qnx4/inode.c#L266">266</a> 
<a name="L267" href="source/fs/qnx4/inode.c#L267">267</a>         <a href="ident?i=inode">inode</a> = <a href="ident?i=iget_locked">iget_locked</a>(<a href="ident?i=sb">sb</a>, <a href="ident?i=ino">ino</a>);
<a name="L268" href="source/fs/qnx4/inode.c#L268">268</a>         if (!<a href="ident?i=inode">inode</a>)
<a name="L269" href="source/fs/qnx4/inode.c#L269">269</a>                 return <a href="ident?i=ERR_PTR">ERR_PTR</a>(-<a href="ident?i=ENOMEM">ENOMEM</a>);
<a name="L270" href="source/fs/qnx4/inode.c#L270">270</a>         if (!(<a href="ident?i=inode">inode</a>-&gt;i_state &amp; <a href="ident?i=I_NEW">I_NEW</a>))
<a name="L271" href="source/fs/qnx4/inode.c#L271">271</a>                 return <a href="ident?i=inode">inode</a>;
<a name="L272" href="source/fs/qnx4/inode.c#L272">272</a> 
<a name="L273" href="source/fs/qnx4/inode.c#L273">273</a>         qnx4_inode = <a href="ident?i=qnx4_raw_inode">qnx4_raw_inode</a>(<a href="ident?i=inode">inode</a>);
<a name="L274" href="source/fs/qnx4/inode.c#L274">274</a>         <a href="ident?i=inode">inode</a>-&gt;i_mode = 0;
<a name="L275" href="source/fs/qnx4/inode.c#L275">275</a> 
<a name="L276" href="source/fs/qnx4/inode.c#L276">276</a>         <a href="ident?i=QNX4DEBUG">QNX4DEBUG</a>((<a href="ident?i=KERN_INFO">KERN_INFO</a> <i>"reading inode : [%d]\n"</i>, <a href="ident?i=ino">ino</a>));
<a name="L277" href="source/fs/qnx4/inode.c#L277">277</a>         if (!<a href="ident?i=ino">ino</a>) {
<a name="L278" href="source/fs/qnx4/inode.c#L278">278</a>                 <a href="ident?i=printk">printk</a>(<a href="ident?i=KERN_ERR">KERN_ERR</a> <i>"qnx4: bad inode number on dev %s: %lu is "</i>
<a name="L279" href="source/fs/qnx4/inode.c#L279">279</a>                                 <i>"out of range\n"</i>,
<a name="L280" href="source/fs/qnx4/inode.c#L280">280</a>                        <a href="ident?i=sb">sb</a>-&gt;s_id, <a href="ident?i=ino">ino</a>);
<a name="L281" href="source/fs/qnx4/inode.c#L281">281</a>                 <a href="ident?i=iget_failed">iget_failed</a>(<a href="ident?i=inode">inode</a>);
<a name="L282" href="source/fs/qnx4/inode.c#L282">282</a>                 return <a href="ident?i=ERR_PTR">ERR_PTR</a>(-<a href="ident?i=EIO">EIO</a>);
<a name="L283" href="source/fs/qnx4/inode.c#L283">283</a>         }
<a name="L284" href="source/fs/qnx4/inode.c#L284">284</a>         <a href="ident?i=block">block</a> = <a href="ident?i=ino">ino</a> / <a href="ident?i=QNX4_INODES_PER_BLOCK">QNX4_INODES_PER_BLOCK</a>;
<a name="L285" href="source/fs/qnx4/inode.c#L285">285</a> 
<a name="L286" href="source/fs/qnx4/inode.c#L286">286</a>         if (!(<a href="ident?i=bh">bh</a> = <a href="ident?i=sb_bread">sb_bread</a>(<a href="ident?i=sb">sb</a>, <a href="ident?i=block">block</a>))) {
<a name="L287" href="source/fs/qnx4/inode.c#L287">287</a>                 <a href="ident?i=printk">printk</a>(<a href="ident?i=KERN_ERR">KERN_ERR</a> <i>"qnx4: major problem: unable to read inode from dev "</i>
<a name="L288" href="source/fs/qnx4/inode.c#L288">288</a>                        <i>"%s\n"</i>, <a href="ident?i=sb">sb</a>-&gt;s_id);
<a name="L289" href="source/fs/qnx4/inode.c#L289">289</a>                 <a href="ident?i=iget_failed">iget_failed</a>(<a href="ident?i=inode">inode</a>);
<a name="L290" href="source/fs/qnx4/inode.c#L290">290</a>                 return <a href="ident?i=ERR_PTR">ERR_PTR</a>(-<a href="ident?i=EIO">EIO</a>);
<a name="L291" href="source/fs/qnx4/inode.c#L291">291</a>         }
<a name="L292" href="source/fs/qnx4/inode.c#L292">292</a>         raw_inode = ((struct <a href="ident?i=qnx4_inode_entry">qnx4_inode_entry</a> *) <a href="ident?i=bh">bh</a>-&gt;b_data) +
<a name="L293" href="source/fs/qnx4/inode.c#L293">293</a>             (<a href="ident?i=ino">ino</a> % <a href="ident?i=QNX4_INODES_PER_BLOCK">QNX4_INODES_PER_BLOCK</a>);
<a name="L294" href="source/fs/qnx4/inode.c#L294">294</a> 
<a name="L295" href="source/fs/qnx4/inode.c#L295">295</a>         <a href="ident?i=inode">inode</a>-&gt;i_mode    = <a href="ident?i=le16_to_cpu">le16_to_cpu</a>(raw_inode-&gt;di_mode);
<a name="L296" href="source/fs/qnx4/inode.c#L296">296</a>         <a href="ident?i=i_uid_write">i_uid_write</a>(<a href="ident?i=inode">inode</a>, (<a href="ident?i=uid_t">uid_t</a>)<a href="ident?i=le16_to_cpu">le16_to_cpu</a>(raw_inode-&gt;di_uid));
<a name="L297" href="source/fs/qnx4/inode.c#L297">297</a>         <a href="ident?i=i_gid_write">i_gid_write</a>(<a href="ident?i=inode">inode</a>, (<a href="ident?i=gid_t">gid_t</a>)<a href="ident?i=le16_to_cpu">le16_to_cpu</a>(raw_inode-&gt;di_gid));
<a name="L298" href="source/fs/qnx4/inode.c#L298">298</a>         <a href="ident?i=set_nlink">set_nlink</a>(<a href="ident?i=inode">inode</a>, <a href="ident?i=le16_to_cpu">le16_to_cpu</a>(raw_inode-&gt;di_nlink));
<a name="L299" href="source/fs/qnx4/inode.c#L299">299</a>         <a href="ident?i=inode">inode</a>-&gt;i_size    = <a href="ident?i=le32_to_cpu">le32_to_cpu</a>(raw_inode-&gt;di_size);
<a name="L300" href="source/fs/qnx4/inode.c#L300">300</a>         <a href="ident?i=inode">inode</a>-&gt;i_mtime.tv_sec   = <a href="ident?i=le32_to_cpu">le32_to_cpu</a>(raw_inode-&gt;di_mtime);
<a name="L301" href="source/fs/qnx4/inode.c#L301">301</a>         <a href="ident?i=inode">inode</a>-&gt;i_mtime.tv_nsec = 0;
<a name="L302" href="source/fs/qnx4/inode.c#L302">302</a>         <a href="ident?i=inode">inode</a>-&gt;i_atime.tv_sec   = <a href="ident?i=le32_to_cpu">le32_to_cpu</a>(raw_inode-&gt;di_atime);
<a name="L303" href="source/fs/qnx4/inode.c#L303">303</a>         <a href="ident?i=inode">inode</a>-&gt;i_atime.tv_nsec = 0;
<a name="L304" href="source/fs/qnx4/inode.c#L304">304</a>         <a href="ident?i=inode">inode</a>-&gt;i_ctime.tv_sec   = <a href="ident?i=le32_to_cpu">le32_to_cpu</a>(raw_inode-&gt;di_ctime);
<a name="L305" href="source/fs/qnx4/inode.c#L305">305</a>         <a href="ident?i=inode">inode</a>-&gt;i_ctime.tv_nsec = 0;
<a name="L306" href="source/fs/qnx4/inode.c#L306">306</a>         <a href="ident?i=inode">inode</a>-&gt;i_blocks  = <a href="ident?i=le32_to_cpu">le32_to_cpu</a>(raw_inode-&gt;di_first_xtnt.xtnt_size);
<a name="L307" href="source/fs/qnx4/inode.c#L307">307</a> 
<a name="L308" href="source/fs/qnx4/inode.c#L308">308</a>         <a href="ident?i=memcpy">memcpy</a>(qnx4_inode, raw_inode, <a href="ident?i=QNX4_DIR_ENTRY_SIZE">QNX4_DIR_ENTRY_SIZE</a>);
<a name="L309" href="source/fs/qnx4/inode.c#L309">309</a>         if (<a href="ident?i=S_ISREG">S_ISREG</a>(<a href="ident?i=inode">inode</a>-&gt;i_mode)) {
<a name="L310" href="source/fs/qnx4/inode.c#L310">310</a>                 <a href="ident?i=inode">inode</a>-&gt;i_fop = &amp;<a href="ident?i=generic_ro_fops">generic_ro_fops</a>;
<a name="L311" href="source/fs/qnx4/inode.c#L311">311</a>                 <a href="ident?i=inode">inode</a>-&gt;i_mapping-&gt;a_ops = &amp;<a href="ident?i=qnx4_aops">qnx4_aops</a>;
<a name="L312" href="source/fs/qnx4/inode.c#L312">312</a>                 <a href="ident?i=qnx4_i">qnx4_i</a>(<a href="ident?i=inode">inode</a>)-&gt;mmu_private = <a href="ident?i=inode">inode</a>-&gt;i_size;
<a name="L313" href="source/fs/qnx4/inode.c#L313">313</a>         } else if (<a href="ident?i=S_ISDIR">S_ISDIR</a>(<a href="ident?i=inode">inode</a>-&gt;i_mode)) {
<a name="L314" href="source/fs/qnx4/inode.c#L314">314</a>                 <a href="ident?i=inode">inode</a>-&gt;i_op = &amp;<a href="ident?i=qnx4_dir_inode_operations">qnx4_dir_inode_operations</a>;
<a name="L315" href="source/fs/qnx4/inode.c#L315">315</a>                 <a href="ident?i=inode">inode</a>-&gt;i_fop = &amp;<a href="ident?i=qnx4_dir_operations">qnx4_dir_operations</a>;
<a name="L316" href="source/fs/qnx4/inode.c#L316">316</a>         } else if (<a href="ident?i=S_ISLNK">S_ISLNK</a>(<a href="ident?i=inode">inode</a>-&gt;i_mode)) {
<a name="L317" href="source/fs/qnx4/inode.c#L317">317</a>                 <a href="ident?i=inode">inode</a>-&gt;i_op = &amp;<a href="ident?i=page_symlink_inode_operations">page_symlink_inode_operations</a>;
<a name="L318" href="source/fs/qnx4/inode.c#L318">318</a>                 <a href="ident?i=inode">inode</a>-&gt;i_mapping-&gt;a_ops = &amp;<a href="ident?i=qnx4_aops">qnx4_aops</a>;
<a name="L319" href="source/fs/qnx4/inode.c#L319">319</a>                 <a href="ident?i=qnx4_i">qnx4_i</a>(<a href="ident?i=inode">inode</a>)-&gt;mmu_private = <a href="ident?i=inode">inode</a>-&gt;i_size;
<a name="L320" href="source/fs/qnx4/inode.c#L320">320</a>         } else {
<a name="L321" href="source/fs/qnx4/inode.c#L321">321</a>                 <a href="ident?i=printk">printk</a>(<a href="ident?i=KERN_ERR">KERN_ERR</a> <i>"qnx4: bad inode %lu on dev %s\n"</i>,
<a name="L322" href="source/fs/qnx4/inode.c#L322">322</a>                         <a href="ident?i=ino">ino</a>, <a href="ident?i=sb">sb</a>-&gt;s_id);
<a name="L323" href="source/fs/qnx4/inode.c#L323">323</a>                 <a href="ident?i=iget_failed">iget_failed</a>(<a href="ident?i=inode">inode</a>);
<a name="L324" href="source/fs/qnx4/inode.c#L324">324</a>                 <a href="ident?i=brelse">brelse</a>(<a href="ident?i=bh">bh</a>);
<a name="L325" href="source/fs/qnx4/inode.c#L325">325</a>                 return <a href="ident?i=ERR_PTR">ERR_PTR</a>(-<a href="ident?i=EIO">EIO</a>);
<a name="L326" href="source/fs/qnx4/inode.c#L326">326</a>         }
<a name="L327" href="source/fs/qnx4/inode.c#L327">327</a>         <a href="ident?i=brelse">brelse</a>(<a href="ident?i=bh">bh</a>);
<a name="L328" href="source/fs/qnx4/inode.c#L328">328</a>         <a href="ident?i=unlock_new_inode">unlock_new_inode</a>(<a href="ident?i=inode">inode</a>);
<a name="L329" href="source/fs/qnx4/inode.c#L329">329</a>         return <a href="ident?i=inode">inode</a>;
<a name="L330" href="source/fs/qnx4/inode.c#L330">330</a> }
<a name="L331" href="source/fs/qnx4/inode.c#L331">331</a> 
<a name="L332" href="source/fs/qnx4/inode.c#L332">332</a> static struct <a href="ident?i=kmem_cache">kmem_cache</a> *<a href="ident?i=qnx4_inode_cachep">qnx4_inode_cachep</a>;
<a name="L333" href="source/fs/qnx4/inode.c#L333">333</a> 
<a name="L334" href="source/fs/qnx4/inode.c#L334">334</a> static struct <a href="ident?i=inode">inode</a> *<a href="ident?i=qnx4_alloc_inode">qnx4_alloc_inode</a>(struct <a href="ident?i=super_block">super_block</a> *<a href="ident?i=sb">sb</a>)
<a name="L335" href="source/fs/qnx4/inode.c#L335">335</a> {
<a name="L336" href="source/fs/qnx4/inode.c#L336">336</a>         struct <a href="ident?i=qnx4_inode_info">qnx4_inode_info</a> *ei;
<a name="L337" href="source/fs/qnx4/inode.c#L337">337</a>         ei = <a href="ident?i=kmem_cache_alloc">kmem_cache_alloc</a>(<a href="ident?i=qnx4_inode_cachep">qnx4_inode_cachep</a>, <a href="ident?i=GFP_KERNEL">GFP_KERNEL</a>);
<a name="L338" href="source/fs/qnx4/inode.c#L338">338</a>         if (!ei)
<a name="L339" href="source/fs/qnx4/inode.c#L339">339</a>                 return <a href="ident?i=NULL">NULL</a>;
<a name="L340" href="source/fs/qnx4/inode.c#L340">340</a>         return &amp;ei-&gt;vfs_inode;
<a name="L341" href="source/fs/qnx4/inode.c#L341">341</a> }
<a name="L342" href="source/fs/qnx4/inode.c#L342">342</a> 
<a name="L343" href="source/fs/qnx4/inode.c#L343">343</a> static void <a href="ident?i=qnx4_i_callback">qnx4_i_callback</a>(struct <a href="ident?i=rcu_head">rcu_head</a> *<a href="ident?i=head">head</a>)
<a name="L344" href="source/fs/qnx4/inode.c#L344">344</a> {
<a name="L345" href="source/fs/qnx4/inode.c#L345">345</a>         struct <a href="ident?i=inode">inode</a> *<a href="ident?i=inode">inode</a> = <a href="ident?i=container_of">container_of</a>(<a href="ident?i=head">head</a>, struct <a href="ident?i=inode">inode</a>, i_rcu);
<a name="L346" href="source/fs/qnx4/inode.c#L346">346</a>         <a href="ident?i=kmem_cache_free">kmem_cache_free</a>(<a href="ident?i=qnx4_inode_cachep">qnx4_inode_cachep</a>, <a href="ident?i=qnx4_i">qnx4_i</a>(<a href="ident?i=inode">inode</a>));
<a name="L347" href="source/fs/qnx4/inode.c#L347">347</a> }
<a name="L348" href="source/fs/qnx4/inode.c#L348">348</a> 
<a name="L349" href="source/fs/qnx4/inode.c#L349">349</a> static void <a href="ident?i=qnx4_destroy_inode">qnx4_destroy_inode</a>(struct <a href="ident?i=inode">inode</a> *<a href="ident?i=inode">inode</a>)
<a name="L350" href="source/fs/qnx4/inode.c#L350">350</a> {
<a name="L351" href="source/fs/qnx4/inode.c#L351">351</a>         <a href="ident?i=call_rcu">call_rcu</a>(&amp;<a href="ident?i=inode">inode</a>-&gt;i_rcu, <a href="ident?i=qnx4_i_callback">qnx4_i_callback</a>);
<a name="L352" href="source/fs/qnx4/inode.c#L352">352</a> }
<a name="L353" href="source/fs/qnx4/inode.c#L353">353</a> 
<a name="L354" href="source/fs/qnx4/inode.c#L354">354</a> static void <a href="ident?i=init_once">init_once</a>(void *<a href="ident?i=foo">foo</a>)
<a name="L355" href="source/fs/qnx4/inode.c#L355">355</a> {
<a name="L356" href="source/fs/qnx4/inode.c#L356">356</a>         struct <a href="ident?i=qnx4_inode_info">qnx4_inode_info</a> *ei = (struct <a href="ident?i=qnx4_inode_info">qnx4_inode_info</a> *) <a href="ident?i=foo">foo</a>;
<a name="L357" href="source/fs/qnx4/inode.c#L357">357</a> 
<a name="L358" href="source/fs/qnx4/inode.c#L358">358</a>         <a href="ident?i=inode_init_once">inode_init_once</a>(&amp;ei-&gt;vfs_inode);
<a name="L359" href="source/fs/qnx4/inode.c#L359">359</a> }
<a name="L360" href="source/fs/qnx4/inode.c#L360">360</a> 
<a name="L361" href="source/fs/qnx4/inode.c#L361">361</a> static int <a href="ident?i=init_inodecache">init_inodecache</a>(void)
<a name="L362" href="source/fs/qnx4/inode.c#L362">362</a> {
<a name="L363" href="source/fs/qnx4/inode.c#L363">363</a>         <a href="ident?i=qnx4_inode_cachep">qnx4_inode_cachep</a> = <a href="ident?i=kmem_cache_create">kmem_cache_create</a>(<i>"qnx4_inode_cache"</i>,
<a name="L364" href="source/fs/qnx4/inode.c#L364">364</a>                                              sizeof(struct <a href="ident?i=qnx4_inode_info">qnx4_inode_info</a>),
<a name="L365" href="source/fs/qnx4/inode.c#L365">365</a>                                              0, (<a href="ident?i=SLAB_RECLAIM_ACCOUNT">SLAB_RECLAIM_ACCOUNT</a>|
<a name="L366" href="source/fs/qnx4/inode.c#L366">366</a>                                                 <a href="ident?i=SLAB_MEM_SPREAD">SLAB_MEM_SPREAD</a>),
<a name="L367" href="source/fs/qnx4/inode.c#L367">367</a>                                              <a href="ident?i=init_once">init_once</a>);
<a name="L368" href="source/fs/qnx4/inode.c#L368">368</a>         if (<a href="ident?i=qnx4_inode_cachep">qnx4_inode_cachep</a> == <a href="ident?i=NULL">NULL</a>)
<a name="L369" href="source/fs/qnx4/inode.c#L369">369</a>                 return -<a href="ident?i=ENOMEM">ENOMEM</a>;
<a name="L370" href="source/fs/qnx4/inode.c#L370">370</a>         return 0;
<a name="L371" href="source/fs/qnx4/inode.c#L371">371</a> }
<a name="L372" href="source/fs/qnx4/inode.c#L372">372</a> 
<a name="L373" href="source/fs/qnx4/inode.c#L373">373</a> static void <a href="ident?i=destroy_inodecache">destroy_inodecache</a>(void)
<a name="L374" href="source/fs/qnx4/inode.c#L374">374</a> {
<a name="L375" href="source/fs/qnx4/inode.c#L375">375</a>         <b><i>/*</i></b>
<a name="L376" href="source/fs/qnx4/inode.c#L376">376</a> <b><i>         * Make sure all delayed rcu free inodes are flushed before we</i></b>
<a name="L377" href="source/fs/qnx4/inode.c#L377">377</a> <b><i>         * destroy cache.</i></b>
<a name="L378" href="source/fs/qnx4/inode.c#L378">378</a> <b><i>         */</i></b>
<a name="L379" href="source/fs/qnx4/inode.c#L379">379</a>         <a href="ident?i=rcu_barrier">rcu_barrier</a>();
<a name="L380" href="source/fs/qnx4/inode.c#L380">380</a>         <a href="ident?i=kmem_cache_destroy">kmem_cache_destroy</a>(<a href="ident?i=qnx4_inode_cachep">qnx4_inode_cachep</a>);
<a name="L381" href="source/fs/qnx4/inode.c#L381">381</a> }
<a name="L382" href="source/fs/qnx4/inode.c#L382">382</a> 
<a name="L383" href="source/fs/qnx4/inode.c#L383">383</a> static struct <a href="ident?i=dentry">dentry</a> *<a href="ident?i=qnx4_mount">qnx4_mount</a>(struct <a href="ident?i=file_system_type">file_system_type</a> *<a href="ident?i=fs_type">fs_type</a>,
<a name="L384" href="source/fs/qnx4/inode.c#L384">384</a>         int <a href="ident?i=flags">flags</a>, const char *<a href="ident?i=dev_name">dev_name</a>, void *<a href="ident?i=data">data</a>)
<a name="L385" href="source/fs/qnx4/inode.c#L385">385</a> {
<a name="L386" href="source/fs/qnx4/inode.c#L386">386</a>         return <a href="ident?i=mount_bdev">mount_bdev</a>(<a href="ident?i=fs_type">fs_type</a>, <a href="ident?i=flags">flags</a>, <a href="ident?i=dev_name">dev_name</a>, <a href="ident?i=data">data</a>, <a href="ident?i=qnx4_fill_super">qnx4_fill_super</a>);
<a name="L387" href="source/fs/qnx4/inode.c#L387">387</a> }
<a name="L388" href="source/fs/qnx4/inode.c#L388">388</a> 
<a name="L389" href="source/fs/qnx4/inode.c#L389">389</a> static struct <a href="ident?i=file_system_type">file_system_type</a> <a href="ident?i=qnx4_fs_type">qnx4_fs_type</a> = {
<a name="L390" href="source/fs/qnx4/inode.c#L390">390</a>         .owner          = <a href="ident?i=THIS_MODULE">THIS_MODULE</a>,
<a name="L391" href="source/fs/qnx4/inode.c#L391">391</a>         .<a href="ident?i=name">name</a>           = <i>"qnx4"</i>,
<a name="L392" href="source/fs/qnx4/inode.c#L392">392</a>         .<a href="ident?i=mount">mount</a>          = <a href="ident?i=qnx4_mount">qnx4_mount</a>,
<a name="L393" href="source/fs/qnx4/inode.c#L393">393</a>         .kill_sb        = <a href="ident?i=qnx4_kill_sb">qnx4_kill_sb</a>,
<a name="L394" href="source/fs/qnx4/inode.c#L394">394</a>         .fs_flags       = <a href="ident?i=FS_REQUIRES_DEV">FS_REQUIRES_DEV</a>,
<a name="L395" href="source/fs/qnx4/inode.c#L395">395</a> };
<a name="L396" href="source/fs/qnx4/inode.c#L396">396</a> <a href="ident?i=MODULE_ALIAS_FS">MODULE_ALIAS_FS</a>(<i>"qnx4"</i>);
<a name="L397" href="source/fs/qnx4/inode.c#L397">397</a> 
<a name="L398" href="source/fs/qnx4/inode.c#L398">398</a> static int <a href="ident?i=__init">__init</a> <a href="ident?i=init_qnx4_fs">init_qnx4_fs</a>(void)
<a name="L399" href="source/fs/qnx4/inode.c#L399">399</a> {
<a name="L400" href="source/fs/qnx4/inode.c#L400">400</a>         int <a href="ident?i=err">err</a>;
<a name="L401" href="source/fs/qnx4/inode.c#L401">401</a> 
<a name="L402" href="source/fs/qnx4/inode.c#L402">402</a>         <a href="ident?i=err">err</a> = <a href="ident?i=init_inodecache">init_inodecache</a>();
<a name="L403" href="source/fs/qnx4/inode.c#L403">403</a>         if (<a href="ident?i=err">err</a>)
<a name="L404" href="source/fs/qnx4/inode.c#L404">404</a>                 return <a href="ident?i=err">err</a>;
<a name="L405" href="source/fs/qnx4/inode.c#L405">405</a> 
<a name="L406" href="source/fs/qnx4/inode.c#L406">406</a>         <a href="ident?i=err">err</a> = <a href="ident?i=register_filesystem">register_filesystem</a>(&amp;<a href="ident?i=qnx4_fs_type">qnx4_fs_type</a>);
<a name="L407" href="source/fs/qnx4/inode.c#L407">407</a>         if (<a href="ident?i=err">err</a>) {
<a name="L408" href="source/fs/qnx4/inode.c#L408">408</a>                 <a href="ident?i=destroy_inodecache">destroy_inodecache</a>();
<a name="L409" href="source/fs/qnx4/inode.c#L409">409</a>                 return <a href="ident?i=err">err</a>;
<a name="L410" href="source/fs/qnx4/inode.c#L410">410</a>         }
<a name="L411" href="source/fs/qnx4/inode.c#L411">411</a> 
<a name="L412" href="source/fs/qnx4/inode.c#L412">412</a>         <a href="ident?i=printk">printk</a>(<a href="ident?i=KERN_INFO">KERN_INFO</a> <i>"QNX4 filesystem 0.2.3 registered.\n"</i>);
<a name="L413" href="source/fs/qnx4/inode.c#L413">413</a>         return 0;
<a name="L414" href="source/fs/qnx4/inode.c#L414">414</a> }
<a name="L415" href="source/fs/qnx4/inode.c#L415">415</a> 
<a name="L416" href="source/fs/qnx4/inode.c#L416">416</a> static void <a href="ident?i=__exit">__exit</a> <a href="ident?i=exit_qnx4_fs">exit_qnx4_fs</a>(void)
<a name="L417" href="source/fs/qnx4/inode.c#L417">417</a> {
<a name="L418" href="source/fs/qnx4/inode.c#L418">418</a>         <a href="ident?i=unregister_filesystem">unregister_filesystem</a>(&amp;<a href="ident?i=qnx4_fs_type">qnx4_fs_type</a>);
<a name="L419" href="source/fs/qnx4/inode.c#L419">419</a>         <a href="ident?i=destroy_inodecache">destroy_inodecache</a>();
<a name="L420" href="source/fs/qnx4/inode.c#L420">420</a> }
<a name="L421" href="source/fs/qnx4/inode.c#L421">421</a> 
<a name="L422" href="source/fs/qnx4/inode.c#L422">422</a> <a href="ident?i=module_init">module_init</a>(<a href="ident?i=init_qnx4_fs">init_qnx4_fs</a>)
<a name="L423" href="source/fs/qnx4/inode.c#L423">423</a> <a href="ident?i=module_exit">module_exit</a>(<a href="ident?i=exit_qnx4_fs">exit_qnx4_fs</a>)
<a name="L424" href="source/fs/qnx4/inode.c#L424">424</a> <a href="ident?i=MODULE_LICENSE">MODULE_LICENSE</a>(<i>"GPL"</i>);
<a name="L425" href="source/fs/qnx4/inode.c#L425">425</a> 
<a name="L426" href="source/fs/qnx4/inode.c#L426">426</a> </pre></div><p>
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
