<?xml version="1.0" encoding="UTF-8"?>
<tileset name="crab" tilewidth="32" tileheight="32" tilecount="80" columns="8">
 <image source="crab.png" width="256" height="320"/>
 <tile id="0">
  <properties>
   <property name="state" value="death"/>
  </properties>
  <animation>
   <frame tileid="0" duration="100"/>
   <frame tileid="2" duration="100"/>
   <frame tileid="4" duration="100"/>
   <frame tileid="6" duration="100"/>
  </animation>
 </tile>
 <tile id="8">
  <properties>
   <property name="state" value="attack_r"/>
  </properties>
  <animation>
   <frame tileid="8" duration="100"/>
   <frame tileid="9" duration="100"/>
   <frame tileid="10" duration="100"/>
   <frame tileid="11" duration="100"/>
   <frame tileid="9" duration="100"/>
  </animation>
 </tile>
 <tile id="16">
  <properties>
   <property name="state" value="walk_down"/>
  </properties>
  <animation>
   <frame tileid="16" duration="100"/>
   <frame tileid="17" duration="100"/>
   <frame tileid="18" duration="100"/>
   <frame tileid="19" duration="100"/>
   <frame tileid="20" duration="100"/>
   <frame tileid="21" duration="100"/>
  </animation>
 </tile>
 <tile id="24">
  <properties>
   <property name="state" value="idle_r"/>
  </properties>
  <animation>
   <frame tileid="8" duration="100"/>
   <frame tileid="25" duration="100"/>
   <frame tileid="8" duration="100"/>
   <frame tileid="27" duration="100"/>
  </animation>
 </tile>
 <tile id="40">
  <properties>
   <property name="state" value="idle_up"/>
  </properties>
  <animation>
   <frame tileid="32" duration="100"/>
   <frame tileid="41" duration="100"/>
   <frame tileid="32" duration="100"/>
   <frame tileid="43" duration="100"/>
  </animation>
 </tile>
 <tile id="48">
  <properties>
   <property name="state" value="walk_left"/>
  </properties>
  <animation>
   <frame tileid="48" duration="100"/>
   <frame tileid="49" duration="100"/>
   <frame tileid="50" duration="100"/>
  </animation>
 </tile>
</tileset>
