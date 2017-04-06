<?xml version="1.0" encoding="UTF-8"?>
<tileset name="guard" tilewidth="25" tileheight="24" tilecount="4" columns="2">
 <image source="guard_25x24.png" width="50" height="48"/>
 <tile id="0">
  <properties>
   <property name="state" value="idle"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="7" y="6" width="11" height="15"/>
  </objectgroup>
  <animation>
   <frame tileid="0" duration="500"/>
   <frame tileid="1" duration="500"/>
  </animation>
 </tile>
 <tile id="2">
  <properties>
   <property name="state" value="idle_nuit"/>
  </properties>
  <objectgroup draworder="index">
   <object id="1" x="8" y="11" width="10" height="12"/>
  </objectgroup>
  <animation>
   <frame tileid="2" duration="250"/>
   <frame tileid="3" duration="250"/>
  </animation>
 </tile>
</tileset>
