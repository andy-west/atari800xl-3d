using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace ObjToAtari
{
    public class Program
    {
        public static void Main()
        {
            var vertices = new List<Vertex>();
            var currentColor = string.Empty;
            var triangles = new List<Triangle>();

            var linesOfText = File.ReadAllLines("city-block.obj");

            var minX = double.MaxValue;
            var maxX = double.MinValue;
            var minY = double.MaxValue;
            var maxY = double.MinValue;
            var minZ = double.MaxValue;
            var maxZ = double.MinValue;

            foreach (var line in linesOfText)
            {
                switch (line[0])
                {
                    case 'v':
                        var vertexCoordinates = line.Split(' ').Skip(1).Select(double.Parse).ToArray();

                        var v0 = vertexCoordinates[0];
                        var v1 = vertexCoordinates[1];
                        var v2 = vertexCoordinates[2];

                        minX = Math.Min(minX, v0);
                        minY = Math.Min(minY, v1);
                        minZ = Math.Min(minZ, v2);

                        maxX = Math.Max(maxX, v0);
                        maxY = Math.Max(maxY, v1);
                        maxZ = Math.Max(maxZ, v2);

                        vertices.Add(new Vertex(vertexCoordinates[0], vertexCoordinates[1], vertexCoordinates[2]));
                        break;
                    case 'g':
                        currentColor = line.Split('_').Last();
                        break;
                    case 'f':
                        var triangleVertices = line.Split(' ').Skip(1).Select(int.Parse).ToArray();

                        var triangle = new Triangle(
                            vertices[triangleVertices[0] - 1],
                            vertices[triangleVertices[1] - 1],
                            vertices[triangleVertices[2] - 1],
                            currentColor);

                        triangles.Add(triangle);
                        break;
                }
            }

            triangles = triangles.OrderBy(t => t.Color).ToList();

            var scaleX = 16000.0d / (maxX - minX);
            var scaleY = 16000.0d / (maxY - minY);
            var scaleZ = 16000.0d / (maxZ - minZ);

            var scale = Math.Min(scaleX, scaleY);
            scale = Math.Min(scale, scaleZ);

            var bytes = new List<byte>();

            bytes.AddRange(new[] { (byte)'B', (byte)135, (byte)206, (byte)235 });
            bytes.Add((byte)'D');

            var previousColor = string.Empty;

            foreach (var t in triangles)
            {
                if (t.Color != previousColor)
                {
                    var r = Convert.ToByte(t.Color.Substring(0, 2), 16);
                    var g = Convert.ToByte(t.Color.Substring(2, 2), 16);
                    var b = Convert.ToByte(t.Color.Substring(4, 2), 16);

                    bytes.AddRange(new[] { (byte)'C', r, g, b });
                    previousColor = t.Color;
                }

                var v0X = (ushort)((t.V0.X - minX) * scale);
                var v0Y = (ushort)((t.V0.Y - minY) * scale);
                var v0Z = (ushort)((t.V0.Z - minZ) * scale);

                var v1X = (ushort)((t.V1.X - minX) * scale);
                var v1Y = (ushort)((t.V1.Y - minY) * scale);
                var v1Z = (ushort)((t.V1.Z - minZ) * scale);

                var v2X = (ushort)((t.V2.X - minX) * scale);
                var v2Y = (ushort)((t.V2.Y - minY) * scale);
                var v2Z = (ushort)((t.V2.Z - minZ) * scale);

                bytes.AddRange(new[]
                {
                    (byte)'T',
                    GetHighByte(v0X), GetLowByte(v0X), GetHighByte(v0Y), GetLowByte(v0Y), GetHighByte(v0Z), GetLowByte(v0Z),
                    GetHighByte(v1X), GetLowByte(v1X), GetHighByte(v1Y), GetLowByte(v1Y), GetHighByte(v1Z), GetLowByte(v1Z),
                    GetHighByte(v2X), GetLowByte(v2X), GetHighByte(v2Y), GetLowByte(v2Y), GetHighByte(v2Z), GetLowByte(v2Z)
                });
            }

            bytes.Add((byte)'E');

            File.WriteAllBytes("city-block.bin", bytes.ToArray());
        }

        private static byte GetHighByte(ushort i) => (byte)(i >> 8);

        private static byte GetLowByte(ushort i) => (byte)(i & 0xFF);
    }
}
