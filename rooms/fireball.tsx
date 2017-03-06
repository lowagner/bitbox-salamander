<?xml version="1.0" encoding="UTF-8"?>
<tileset name="fireball" tilewidth="16" tileheight="16" tilecount="16" columns="8">
 <image source="fireball.png" trans="ff00da" width="128" height="32"/>
 <tile id="0">
  <properties>
   <property name="state" value="spawn"/>
  </properties>
  <animation>
   <frame tileid="0" duration="100"/>
   <frame tileid="1" duration="100"/>
   <frame tileid="2" duration="100"/>
   <frame tileid="3" duration="100"/>
   <frame tileid="4" duration="100"/>
  </animation>
 </tile>
 <tile id="4">
  <properties>
   <property name="state" value="idle"/>
  </properties>
  <animation>
   <frame tileid="4" duration="100"/>
   <frame tileid="6" duration="100"/>
   <frame tileid="4" duration="100"/>
   <frame tileid="5" duration="100"/>
   <frame tileid="6" duration="100"/>
   <frame tileid="7" duration="100"/>
   <frame tileid="5" duration="100"/>
  </animation>
 </tile>
</tileset>
