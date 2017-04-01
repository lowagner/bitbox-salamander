<?xml version="1.0" encoding="UTF-8"?>
<tileset name="boss" tilewidth="64" tileheight="72" tilecount="60" columns="6">
 <image source="boss.png" width="384" height="720"/>
 <tile id="0">
  <properties>
   <property name="state" value="attack_r"/>
  </properties>
  <animation>
   <frame tileid="0" duration="100"/>
   <frame tileid="1" duration="100"/>
   <frame tileid="2" duration="100"/>
   <frame tileid="3" duration="100"/>
   <frame tileid="4" duration="100"/>
   <frame tileid="5" duration="100"/>
  </animation>
 </tile>
 <tile id="6">
  <properties>
   <property name="state" value="walk_r"/>
  </properties>
  <animation>
   <frame tileid="6" duration="100"/>
   <frame tileid="7" duration="100"/>
   <frame tileid="8" duration="100"/>
   <frame tileid="7" duration="100"/>
  </animation>
 </tile>
 <tile id="12">
  <properties>
   <property name="state" value="idle_r"/>
  </properties>
  <animation>
   <frame tileid="7" duration="100"/>
   <frame tileid="7" duration="100"/>
   <frame tileid="13" duration="100"/>
  </animation>
 </tile>
 <tile id="18">
  <properties>
   <property name="state" value="attack_up"/>
  </properties>
  <animation>
   <frame tileid="18" duration="100"/>
   <frame tileid="22" duration="100"/>
   <frame tileid="20" duration="100"/>
   <frame tileid="20" duration="100"/>
   <frame tileid="19" duration="100"/>
  </animation>
 </tile>
 <tile id="24">
  <properties>
   <property name="state" value="walk_up"/>
  </properties>
  <animation>
   <frame tileid="24" duration="100"/>
   <frame tileid="25" duration="100"/>
   <frame tileid="24" duration="100"/>
   <frame tileid="27" duration="100"/>
  </animation>
 </tile>
 <tile id="30">
  <properties>
   <property name="state" value="idle_up"/>
  </properties>
  <animation>
   <frame tileid="24" duration="100"/>
   <frame tileid="31" duration="100"/>
  </animation>
 </tile>
 <tile id="36">
  <properties>
   <property name="state" value="attack_down"/>
  </properties>
  <animation>
   <frame tileid="36" duration="100"/>
   <frame tileid="37" duration="100"/>
   <frame tileid="38" duration="100"/>
   <frame tileid="39" duration="100"/>
   <frame tileid="39" duration="100"/>
   <frame tileid="38" duration="100"/>
  </animation>
 </tile>
 <tile id="42">
  <properties>
   <property name="state" value="walk_down"/>
  </properties>
  <animation>
   <frame tileid="37" duration="100"/>
   <frame tileid="43" duration="100"/>
   <frame tileid="37" duration="100"/>
   <frame tileid="45" duration="100"/>
  </animation>
 </tile>
 <tile id="48">
  <properties>
   <property name="state" value="idle_down"/>
  </properties>
  <animation>
   <frame tileid="37" duration="100"/>
   <frame tileid="49" duration="100"/>
  </animation>
 </tile>
</tileset>
