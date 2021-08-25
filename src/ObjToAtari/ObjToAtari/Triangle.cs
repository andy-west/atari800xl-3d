namespace ObjToAtari
{
    public class Triangle
    {
        public Vertex V0 { get; set; }
        public Vertex V1 { get; set; }
        public Vertex V2 { get; set; }
        public string Color { get; set; }

        public Triangle(Vertex v0, Vertex v1, Vertex v2, string color)
        {
            V0 = v0;
            V1 = v1;
            V2 = v2;
            Color = color;
        }
    }
}
