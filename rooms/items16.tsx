<?xml version="1.0" encoding="UTF-8"?>
<tileset name="items16" tilewidth="16" tileheight="16" tilecount="16" columns="4">
 <image source="items16.png" trans="ff00da" width="64" height="64"/>
 <tile id="0">
  <properties>
   <property name="state" value="letter"/>
  </properties>
 </tile>
 <tile id="1">
  <properties>
   <property name="state" value="transp"/>
  </properties>
 </tile>
 <tile id="2">
  <properties>
   <property name="state" value="chest_open"/>
  </properties>
 </tile>
 <tile id="3">
  <properties>
   <property name="state" value="chest_closed"/>
  </properties>
 </tile>
 <tile id="4">
  <properties>
   <property name="state" value="vase"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="1" y="2" width="14" height="13"/>
  </objectgroup>
 </tile>
 <tile id="5">
  <properties>
   <property name="state" value="vasebroken"/>
  </properties>
  <animation>
   <frame tileid="5" duration="100"/>
   <frame tileid="6" duration="100"/>
  </animation>
 </tile>
 <tile id="8">
  <properties>
   <property name="state" value="stick"/>
  </properties>
 </tile>
</tileset>
