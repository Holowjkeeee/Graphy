#ifndef PRESETS_HPP


const char* k6_dot = R"str(graph {
v0 -- v1;
v1 -- v2;
v2 -- v3;
v3 -- v4;
v4 -- v5;
v0 -- v5;
v0 -- v2;
v0 -- v3;
v0 -- v4;
v1 -- v3;
v1 -- v4;
v1 -- v5;
v2 -- v4;
v2 -- v5;
v3 -- v5;
})str";



const char* example = R"str(graph {
v0 -- v1;
v0 -- v4;
v1 -- v2;
v1 -- v5;
v2 -- v6;
v2 -- v3;
v3 -- v7;
v4 -- v5;
v5 -- v6;
v7 -- v11;
v11 -- v10;
v10 -- v6;
v9 -- v5;
v8 -- v9;
v8 -- v4;
v9 -- v10;
v6 -- v7;
v12 -- v8;
v12 -- v13;
v13 -- v9;
v13 -- v14;
v14 -- v10;
v14 -- v15;
v15 -- v11;
})str";




const char* k6_2_dot = R"str(graph {
v0 -- v1;
v1 -- v2;
v2 -- v3;
v3 -- v4;
v4 -- v5;
v5 -- v0;
v5 -- v6;
v6 -- v7;
v7 -- v8;
v8 -- v9;
v9 -- v10;
v10 -- v5;
})str";




const char* small_dense_dot = R"str(graph {
v0 -- v1;
v0 -- v2;
v0 -- v3;
v1 -- v2;
v1 -- v4;
v2 -- v4;
v2 -- v5;
v3 -- v5;
v3 -- v6;
v4 -- v7;
v5 -- v7;
v5 -- v8;
v5 -- v9;
v5 -- v6;
v6 -- v10;
v7 -- v14;
v7 -- v11;
v8 -- v11;
v8 -- v12;
v8 -- v9;
v9 -- v12;
v9 -- v13;
v9 -- v10;
v10 -- v13;
v10 -- v17;
v11 -- v14;
v11 -- v12;
v12 -- v14;
v12 -- v15;
v12 -- v13;
v13 -- v16;
v13 -- v17;
v14 -- v18;
v14 -- v15;
v15 -- v18;
v15 -- v19;
v15 -- v16;
v16 -- v19;
v16 -- v17;
v17 -- v19;
v18 -- v20;
v19 -- v20;
})str";




const char* bin_tree_dot = R"str(graph G {
v0
v0 -- v1
v1
v1 -- v2
v2
v2 -- v3
v2 -- v4
v3
v3 -- v5
v3 -- v6
v4
v4 -- v7
v4 -- v8
v5
v5 -- v9
v5 -- v10
v6
v6 -- v11
v6 -- v12
v7
v7 -- v13
v7 -- v14
v8
v8 -- v15
v9
v9 -- v16
v9 -- v17
v10
v10 -- v18
v10 -- v19
v11
v11 -- v20
v12
v12 -- v21
v13
v13 -- v22
v13 -- v23
v14
v14 -- v24
v15
v15 -- v25
v16
v16 -- v26
v17
v17 -- v27
v18
v18 -- v28
v19
v19 -- v29
v20
v20 -- v30
v21
v21 -- v31
v21 -- v32
v22
v22 -- v33
v22 -- v34
v23
v23 -- v35
v24
v24 -- v36
v24 -- v37
v25
v25 -- v38
v26
v26 -- v39
v27
v27 -- v40
v28
v28 -- v41
v29
v29 -- v42
v29 -- v43
v30
v30 -- v44
v30 -- v45
v31
v31 -- v46
v31 -- v47
v32
v32 -- v48
v33
v33 -- v49
v34
v34 -- v50
v34 -- v51
v35
v35 -- v52
v36
v36 -- v53
v37
v37 -- v54
v38
v38 -- v55
v38 -- v56
v39
v39 -- v57
v39 -- v58
v40
v40 -- v59
v41
v41 -- v60
v41 -- v61
v42
v42 -- v62
v43
v43 -- v63
v44
v44 -- v64
v45
v45 -- v65
v46
v46 -- v66
v47
v47 -- v67
v47 -- v68
v48
v48 -- v69
v49
v49 -- v70
v50
v50 -- v71
v50 -- v72
v51
v51 -- v73
v52
v52 -- v74
v53
v53 -- v75
v54
v54 -- v76
v55
v55 -- v77
v55 -- v78
v56
v56 -- v79
v56 -- v80
v57
v57 -- v81
v57 -- v82
v58
v58 -- v83
v59
v59 -- v84
v60
v60 -- v85
v61
v61 -- v86
v61 -- v87
v62
v62 -- v88
v62 -- v89
v63
v63 -- v90
v63 -- v91
v64
v64 -- v92
v64 -- v93
v65
v65 -- v94
v66
v66 -- v95
v67
v67 -- v96
v68
v68 -- v97
v68 -- v98
v69
v69 -- v99
v70
v71
v72
v73
v74
v75
v76
v77
v78
v79
v80
v81
v82
v83
v84
v85
v86
v87
v88
v89
v90
v91
v92
v93
v94
v95
v96
v97
v98
v99
})str";




const char* quad_tree_dot = R"str(graph G {
v0
v0 -- v1
v0 -- v2
v0 -- v3
v0 -- v4
v1
v1 -- v5
v2
v2 -- v6
v2 -- v7
v3
v3 -- v8
v3 -- v9
v4
v4 -- v10
v4 -- v11
v5
v5 -- v12
v5 -- v13
v6
v6 -- v14
v6 -- v15
v6 -- v16
v6 -- v17
v7
v7 -- v18
v7 -- v19
v7 -- v20
v8
v8 -- v21
v8 -- v22
v8 -- v23
v9
v9 -- v24
v10
v10 -- v25
v11
v11 -- v26
v11 -- v27
v12
v12 -- v28
v12 -- v29
v12 -- v30
v12 -- v31
v13
v13 -- v32
v13 -- v33
v13 -- v34
v14
v14 -- v35
v14 -- v36
v15
v15 -- v37
v15 -- v38
v15 -- v39
v16
v16 -- v40
v17
v17 -- v41
v17 -- v42
v17 -- v43
v18
v18 -- v44
v18 -- v45
v18 -- v46
v19
v19 -- v47
v19 -- v48
v20
v20 -- v49
v20 -- v50
v20 -- v51
v21
v21 -- v52
v22
v22 -- v53
v23
v23 -- v54
v23 -- v55
v24
v24 -- v56
v24 -- v57
v24 -- v58
v25
v25 -- v59
v26
v26 -- v60
v26 -- v61
v27
v27 -- v62
v27 -- v63
v28
v28 -- v64
v28 -- v65
v29
v29 -- v66
v29 -- v67
v29 -- v68
v29 -- v69
v30
v30 -- v70
v30 -- v71
v30 -- v72
v31
v31 -- v73
v31 -- v74
v31 -- v75
v32
v32 -- v76
v33
v33 -- v77
v33 -- v78
v33 -- v79
v34
v34 -- v80
v35
v35 -- v81
v35 -- v82
v36
v36 -- v83
v37
v37 -- v84
v37 -- v85
v37 -- v86
v38
v38 -- v87
v38 -- v88
v38 -- v89
v39
v39 -- v90
v39 -- v91
v39 -- v92
v40
v40 -- v93
v40 -- v94
v41
v41 -- v95
v42
v42 -- v96
v42 -- v97
v43
v43 -- v98
v43 -- v99
v44
v45
v46
v47
v48
v49
v50
v51
v52
v53
v54
v55
v56
v57
v58
v59
v60
v61
v62
v63
v64
v65
v66
v67
v68
v69
v70
v71
v72
v73
v74
v75
v76
v77
v78
v79
v80
v81
v82
v83
v84
v85
v86
v87
v88
v89
v90
v91
v92
v93
v94
v95
v96
v97
v98
v99
})str";

#endif // !PRESETS_HPP
