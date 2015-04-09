# SceneML #

SceneML is a simple 3D scene creation and management tool and also provides fast and accurate collision queries. Currently, collision calculations are provided by the [Open Dynamics Engine](http://www.ode.org/). It also supports a mex interface for easy use inside Matlab.

## Basic description ##

SceneML is a simple 3D scene creation and management tool with its ultimate purpose being a collision query tool. 3D scenes are described in a straight forward XML format. For instance:

```
<?xml version="1.0" encoding="UTF-8"?>

<scene>
	<!-- Source robot (no payload) -->
	<space name="pa10_source" type="simple">
		<!-- Base body, no joints -->
		<body name="src0" parent="world">
			<transform>
				<translation value="0 1200.0 0"/>
				<rotation type="z" value="-pi/2"/>
				<rotation type="y" value="pi/2"/>
			</transform>
			<geometry type="mesh" name="src0_01">
				<parameter type="filename" value="D:/projects/rainbow/trunk/pa10_model/b0_01.obj"/>
				<parameter type="color" value="200 200 200" />
			</geometry>
		</body>
	</space>
</scene>
```

SceneML supports virtually every primitive geometry supported by ODE (e.g. boxes, spheres, capsules, etc) as well as arbitrary user-defined meshes specified in either OBJ or STL file formats. The XML scene format also allows for variable parameters which can be modified after the scene is loaded from file. Anytime a variable parameter is modified, SceneML can recompute the position and orientation of all bodies which are impacted by the change of that parameter. Here is an example of a variable rotation being specified:

```
<rotation type="z" value="0" mutable="1" name="q_sen_3"/>
```

Bodies and geometries are lumped together into "spaces". The user can also specify which spaces the library should check for inter-collisions. For example:

```
<!-- All possible collision pairs which we care about -->
<collisionpairs>
	<pair space1="payloads" space2="pa10_sensor"/>
	<pair space1="payloads" space2="pa10_source"/>
	<pair space1="obstacles" space2="pa10_sensor"/>
	<pair space1="obstacles" space2="pa10_source"/>
	<pair space1="payloads" space2="obstacles"/>
</collisionpairs>
```

Finally here is a screen-shot of SceneML being used in Matlab. As you can see, quite complex 3D scenes can be written without too much fuss.

![http://web.mac.com/yamokosk/J.D.s_site/Images/sceneml_screenshot_400.png](http://web.mac.com/yamokosk/J.D.s_site/Images/sceneml_screenshot_400.png)

## Installation ##

Instructions for installing on Windows, Mac, and Linux can be found [here](SceneML_Installation.md).

## Latest release ##

The latest version of SceneML is version 1.5. See the change log for a list of updates and/or bug fixes.

  * [Version 1.3](SceneML_v_1_3.md)
  * [Version 1.4](SceneML_v_1_4.md)
  * [Version 1.5](SceneML_v_1_5.md)