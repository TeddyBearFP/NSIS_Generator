--Users Table
Create Table if not exists NSISParams (Key TEXT, Value TEXT)
INSERT INTO NSISParams(Key, Value) VALUES('ProgramName', 'TBProgram')
INSERT INTO NSISParams(Key, Value) VALUES('Icon', 'tbICON3_128x128.ico')
INSERT INTO NSISParams(Key, Value) VALUES('Company', 'G.C.L.')
INSERT INTO NSISParams(Key, Value) VALUES('ProgramReleaseFile', 'release.zip')
INSERT INTO NSISParams(Key, Value) VALUES('Version', '1.0.0.1')
--Parameters Table
Create Table if not exists SystemParams (Name text, Value text, PRIMARY KEY(Name))

