boolean event = false;

void setup()
{
  size(1100,900);
}
void draw()
{
  background(65,65,65);
  fill(0,255,0);
   rect(100,100,900,200);
   rect(100,500,900,200);
  
   fill(255,255,255);
   ellipse(200,200,80,80);
   ellipse(300,200,80,80);
   ellipse(400,200,80,80);
   ellipse(500,200,80,80);
   ellipse(600,200,80,80);
   ellipse(700,200,80,80);
   ellipse(800,200,80,80);
   ellipse(900,200,80,80);
   fill(255,0,0);
   ellipse(200,600,80,80);
   ellipse(300,600,80,80);
   ellipse(400,600,80,80);
   ellipse(500,600,80,80);
   ellipse(600,600,80,80);
   ellipse(700,600,80,80);
   ellipse(800,600,80,80);
   ellipse(900,600,80,80);
   
   
   for(int i =0; i<8; i++)
   {
     if(mousePressed && (mouseX > 160+i*100 && mouseX < 240 +i*100) &&(mouseY > 560 && mouseY < 640))
   {
     fill(0,0,0);
     event = true;
     ellipse(200+i*100,200,80,80);
   }
   }
   
   
  
}
