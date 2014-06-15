vstart = #(0,0,0,0,0,0,0,0,0,0)
vend = #(0,0,0,0,0,0,0,0,0,0)
animspeed = #(4,4,4,4,4,4,4,4,4,4)
animloop = #(1,1,1,1,1,1,1,1,1,1)
frames=0 
numanimations=1

rollout CORE_EXPORTER "Core3D Exporter" width:326 height:282
(
	
	label lbl_modeltype "Model type:" pos:[7,7] width:61 height:15
	radiobuttons rb_modeltype "" pos:[73,6] width:142 height:16 labels:#("Static", "Animated") default:1 columns:2
	button btn_export "Export" pos:[5,250] width:93 height:28 
	button btn_close "Exit" pos:[100,250] width:85 height:28
	button btn_name "set" pos:[290,86] width:30 height:20 visible:false
	button btn_start "set" pos:[290,109] width:30 height:19 visible:false
	button btn_end "set" pos:[290,132] width:30 height:19 visible:false
	listbox lb_anims "Animations:" pos:[5,66] width:160 height:12 items:#("Animation 1", "Animation 2", "Animation 3", "Animation 4", "Animation 5", "Animation 6", "Animation 7", "Animation 8", "Animation 9", "Animation 10") visible:false
	edittext txt_framename "Name:" pos:[170,89] width:116 height:19 visible:false
	edittext txt_framestart "Start frame:" pos:[169,112] width:117 height:19 enabled:true readOnly:true visible:false
	edittext txt_frameend "End frame: " pos:[169,135] width:117 height:18 readOnly:true visible:false
	checkbox chk_colorverts "Export Color vertices" pos:[8,47] width:123 height:17 
	checkbox chk_normals "Export Normals" pos:[7,29] width:131 height:17
	checkbox chk_animloop "Loop animation" pos:[169,177] width:94 height:19 visible:false
	spinner spn_speed "Speed:        " pos:[217,156] width:101 height:16 range:[1,100,1] type:#integer scale:1 visible:false
	spinner spn_numanimations "Num. Animations:" pos:[205,49] width:112 height:16 range:[1,100,1] type:#integer scale:1 visible:false
	

	on rb_modeltype changed '1' do
	(
	   if rb_modeltype.state==1 do (
	   btn_start.visible=false	
	   btn_end.visible=false	
	   btn_name.visible=false	
	   txt_framename.visible=false
	   txt_framestart.visible=false
	   txt_frameend.visible=false
	   lb_anims.visible=false
	   spn_speed.visible=false
	   chk_animloop.visible=false
	   spn_numanimations.visible=false
	   )
	   if rb_modeltype.state==2 do (
	   btn_start.visible=true	
	   btn_end.visible=true	
	   btn_name.visible=true	
	   txt_framename.visible=true
	   txt_framestart.visible=true
	   txt_frameend.visible=true
	   lb_anims.visible=true
	   spn_speed.visible=true
	   chk_animloop.visible=true
	   spn_numanimations.visible=true
	   )
	)
	on btn_export pressed do
	(
	   if selection[1] == null do ( messagebox "select a model please"  ) 
	   --EXPORT SELECTED MODEL
	   if selection[1] != null do (
	      tmesh = snapshotAsMesh selection[1]
		  frames = vend[1] as integer - vstart[1] as integer
		  filename = getSaveFileName \	  
		  types:"Core 3D Model(*.c3m)|*.c3m"
	 		
		  if filename!=undefined do (
			 if (rb_modeltype.state==2) do (export_animation=1)
	
	-- =================  START MODEL EXPORT =================
		
	if (chk_colorverts.checked==true) do (export_vertexcolors = 1) 
	if (chk_normals.checked==true) do (export_normals = 1) 
		
	-- count facegroups
	for i = 1 to tmesh.numfaces do (
		matid = getFaceMatId tmesh i
	)
		
	num_verts = tmesh.numverts
	num_facegroups = 1
	num_uvchannels = (meshop.getNumMaps tmesh) - 1
	num_frames = frames
	modelname = selection[1].name
		
	-- open file
	f=fopen filename "wb" 
		
	id=0x4d443343
	flags=32768;   -- highest bit = Version 2.0  
	version=2;
	if (export_normals==1) do flags+=1
	if (export_vertexcolors==1) do flags+=2
	if (export_animation==1) do flags+=4

	if (version==2) do num_verts = tmesh.numfaces*3

	
	-- write header
	WriteLong f id
	WriteShort f num_verts
	WriteShort f num_facegroups
	WriteShort f num_uvchannels
	WriteShort f flags
		
	if (version==2) do  (
		for i = 1 to tmesh.numfaces do (
			face = getFace tmesh i
			a=face.x;b=face.y;c=face.z;

			-- write vertex a
			vert = getVert tmesh a
			WriteFloat f vert.x
			WriteFloat f vert.z
			WriteFloat f -vert.y
			-- write vertex b
			vert = getVert tmesh b
			WriteFloat f vert.x
			WriteFloat f vert.z
			WriteFloat f -vert.y
			-- write vertex c
			vert = getVert tmesh c
			WriteFloat f vert.x
			WriteFloat f vert.z
			WriteFloat f -vert.y
		)
		-- Write faceindex
		for j = 1 to num_facegroups do (
			num_faces =tmesh.numfaces
			matid = getFaceMatId tmesh 0
			WriteShort f num_faces
			WriteShort f matid
			flag=0
			for i = 1 to num_faces do (
				ta=(i-1)*3;
				tb=(i-1)*3+1;
				tc=(i-1)*3+2;
				WriteShort f ta 
				WriteShort f tb
				WriteShort f tc
				WriteShort f flag
			)
		)
		
		num_normals=num_verts
		WriteShort f num_normals
		for i = 1 to tmesh.numfaces do (
			normal = meshop.getFaceRNormals tmesh i		
			-- normal
			WriteFloat f normal[1].x
			WriteFloat f normal[1].y
			WriteFloat f normal[1].z
			WriteFloat f normal[2].x
			WriteFloat f normal[2].y
			WriteFloat f normal[2].z
			WriteFloat f normal[3].x
			WriteFloat f normal[3].y
			WriteFloat f normal[3].z
		)

		num_tverts=num_verts
		WriteShort f num_tverts
		for i = 1 to tmesh.numfaces do (
			cface = meshop.getMapFace tmesh 1 i
			ta=cface.x;tb=cface.y;tc=cface.z;
			-- uvcoord
			cvert = meshop.getMapVert tmesh 1 ta
			WriteFloat f cvert.x 
			WriteFloat f (1.0-cvert.y) 				
			cvert = meshop.getMapVert tmesh 1 tb
			WriteFloat f cvert.x 
			WriteFloat f (1.0-cvert.y) 				
			cvert = meshop.getMapVert tmesh 1 tc
			WriteFloat f cvert.x 
			WriteFloat f (1.0-cvert.y) 							
		)
		
	) 
	if (version==1) do (
	-- OLD VERSION 
	    
		
		-- write vertices and faces
		for i = 1 to num_verts do (
			vert = getVert tmesh i
			vz=-vert.y
			WriteFloat f vert.x
			WriteFloat f vert.z
			WriteFloat f vz
		)
		for j = 1 to num_facegroups do (
			num_faces =tmesh.numfaces
			matid = getFaceMatId tmesh 0
			WriteShort f num_faces
			WriteShort f matid
			for i = 1 to num_faces do (
				face = getFace tmesh i
				sgroup = getFaceSmoothGroup tmesh i
				edge1 = getEdgeVis tmesh i 1
				edge2 = getEdgeVis tmesh i 2
				edge3 = getEdgeVis tmesh i 3
				flags=0
				--sgroup and 255
				if (edge1==true) do (flags=flags+8192)
				if (edge2==true) do (flags=flags+16384)
				if (edge3==true) do (flags=flags+32768)
			
				ta=face.x-1;
				tb=face.y-1;
				tc=face.z-1;
				WriteShort f ta 
				WriteShort f tb
				WriteShort f tc
				WriteShort f flags
			)
		)
		-- UV Channels
		for c = 1 to num_uvchannels do (
			numtverts = meshop.getNumMapVerts tmesh c
			numtfaces = meshop.getNumMapFaces tmesh c
			WriteShort f numtverts
			WriteShort f numtfaces
			
			for cv = 1 to numtverts do (
		 		cvert = meshop.getMapVert tmesh c cv
				u=cvert.x;
				v=1.0-cvert.y;
				WriteFloat f u 
				WriteFloat f v 		
			)
		
			for cf = 1 to numtfaces do (
		 		cface = meshop.getMapFace tmesh c cf
				ta=cface.x-1;
				tb=cface.y-1;
				tc=cface.z-1;
				WriteShort f ta
				WriteShort f tb 
				WriteShort f tc
			)
		) -- channels
	
		-- export normals
		if (export_normals==1) do (
		  WriteShort f num_verts 
		  for i = 1 to num_verts do (
			vert = getNormal tmesh i
			WriteFloat f vert.x
			WriteFloat f vert.y
			WriteFloat f vert.z
		  )
		   --for i = 1 to num_faces do (
		 --	cvface = meshop.getNormalFace tmesh
		--	WriteShort f cvface.x 
		--	WriteShort f cvface.y 
		--	WriteShort f cvface.z
		 --  )
		)
		-- export vertexcolors	
		if (export_vertexcolors==1) do (
		   num_cverts = meshop.getNumMapVerts tmesh 0
		   WriteShort f num_cverts	
		   for i = 1 to num_cverts do (
		 	cvvert = meshop.getMapVert tmesh 0 i
			WriteFloat f cvvert.x 
			WriteFloat f cvvert.y 
			WriteFloat f cvvert.z
		   )
		   for i = 1 to num_faces do (
		 	cvface = meshop.getMapFace tmesh 0 i
			WriteShort f cvface.x 
			WriteShort f cvface.y 
			WriteShort f cvface.z
		   )
		) -- colorverts
		
	
		-- export animation
		if (export_animation==1) do ( 
	   	   --frames = vend[1] as integer - vstart[1] as integer
	   	   --test = frames as string
	
		   animtype=0
		   animflags=0
	
	   	   --num_frames = frames 
		   --num_animations=numanimations as integer;
	  	   WriteShort f numanimations 
		   WriteShort f animtype
		   WriteShort f animflags
	   	   for i6 = 1 to numanimations do (
		       num_frames = vend[i6] as integer - vstart[i6] as integer
		       anim_speed=animspeed[i6];
			   anim_loop=0;
			   if (animloop[i6]==1) do anim_loop=1; 
	
			   WriteShort f num_frames
			   WriteShort f anim_speed
			   WriteShort f anim_loop
			   
		       for i5 = vstart[i6] as integer to vend[i6] as integer do (
		           at time i5
		           tmesh = snapshotAsMesh selection[1]
		           for i2 = 1 to num_verts do (
				      vert = getVert tmesh i2
				      vz=-vert.y
					  WriteFloat f vert.x
				      WriteFloat f vert.z
				      WriteFloat f vz
			      )
		       )
		   ) -- animloop
			
	    ) -- export animation
	 ) -- version 1.0
		fclose f
		messagebox "Model exported" 
	  )--file is defined
	  )--model selected 
	) -- export button (btn_export)
	on btn_close pressed do
	(
	   DestroyDialog CORE_EXPORTER	
	)
	on btn_name pressed do
	(
	  lb_anims.selected = input.text
	)
	on btn_start pressed do
	(
		cur = currentTime as string
		getal1 = trimright cur "f"  
		txt_framestart.text = getal1 
		vstart[lb_anims.selection] = getal1
	)
	on btn_end pressed do
	(
		cur = currentTime as string
		getal1 = trimright cur "f"
		txt_frameend.text =  getal1 
		vend[lb_anims.selection] = getal1
	)
	on lb_anims selected sel do
	(
	   txt_framestart.text= vstart[lb_anims.selection] as string
	   txt_frameend.text= vend[lb_anims.selection] as string
	   spn_speed.value=animspeed[lb_anims.selection] as integer
	   if (animloop[lb_anims.selection]==0) do chk_animloop.checked=false;	   
	   if (animloop[lb_anims.selection]==1) do chk_animloop.checked=true;
	   
	)
	on chk_animloop changed '1' do
	(
	   animloop[lb_anims.selection]=1
	)
	on chk_animloop changed '0' do
	(
	   animloop[lb_anims.selection]=0
	)
	on spn_speed changed val do
	(
		animspeed[lb_anims.selection]=spn_speed.value
	)
	on spn_numanimations changed val do
	(
	   numanimations=spn_numanimations.value
	)
)CreateDialog CORE_EXPORTER
registerViewWindow CORE_EXPORTER



