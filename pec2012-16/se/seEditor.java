import java.awt.*;
import java.awt.event.*;
import java.awt.geom.*;
import javax.swing.*;
import javax.swing.border.*;
import java.util.*;
import java.io.*;
import java.lang.Math.*;
import java.util.StringTokenizer;

enum edit_states
{
    SPAWN,
    DIE,
    MOVE,
    NEXT_LEVEL,
    RESIZE,
    INFO,
    IDLE
}

enum obj_selected
{
    PROC,
    DB,
    ENTITY,
    CONN,
    NOTHING
}

class connection implements Serializable
{
    double X1, X2, Y1, Y2;
    boolean info_added_ltr, info_added_rtl;
    Line2D.Double start, end;
    
    public Vector<String> info_ltr = new Vector<String>(10,2);
    public Vector<String> info_rtl = new Vector<String>(10,2);
    
    private static final long serialVersionUID = 7526499911622776140L;
    
    database start_obj_db, end_obj_db;
    entity start_obj_entity, end_obj_entity;
    process start_obj_process, end_obj_process;
    
    public connection()
    {
        this.start = new Line2D.Double();
        this.end = new Line2D.Double();
        this.info_added_ltr = false;
        this.info_added_rtl = false;
    }
    
    public void select_start_obj_process(process temp)
    {
        this.start_obj_process = temp;
        System.out.println("start object selected as a process");
    }
    
    public void select_start_obj_entity(entity temp)
    {
        this.start_obj_entity = temp;
        System.out.println("start object selected as an entity");
    }
    
    public void select_start_obj_database(database temp)
    {
        this.start_obj_db = temp;
        System.out.println("start object selected as a database");
    }
    
    public void select_end_obj_process(process temp)
    {
        this.end_obj_process = temp;
        System.out.println("end object selected as a process");
    }
    
    public void select_end_obj_entity(entity temp)
    {
        this.end_obj_entity = temp;
        System.out.println("end object selected as an entity");
    }

    public void select_end_obj_database(database temp)
    {
        this.end_obj_db = temp;
        System.out.println("end object selected as a database");
    }
    
    public boolean mycontains_rtl(int xc, int yc)
    {
        if(start.ptSegDist(xc, yc) <= 3)
            return true;
        else
            return false;
    }
    
    public boolean mycontains_ltr(int xc, int yc)
    {
        if(end.ptSegDist(xc, yc) <= 3)
            return true;
        else
            return false;
    }
    
    public boolean mycontains(int xc, int yc)
    {
        if(end.ptSegDist(xc, yc) <= 3 || start.ptSegDist(xc, yc) <= 3)
            return true;
        else
            return false;
    }
    
    public boolean check_feasability()
    {
        if((this.start_obj_entity != null && this.end_obj_entity != null) || (this.start_obj_db != null && this.end_obj_db != null))
        {
            System.out.println("this connection is not possible");
            return false;
        }
        return true;
    }
    
    public boolean check_feasability_2(dfd_level temp_level)
    {
        connection t_conn;
        Iterator<connection> itr = temp_level.list_of_connections.iterator();
        
        while(itr.hasNext())
        {
            t_conn = itr.next();
            if((this.start_obj_process == t_conn.start_obj_process && this.start_obj_db == t_conn.start_obj_db && this.start_obj_entity == t_conn.start_obj_entity && this.end_obj_process == t_conn.end_obj_process && this.end_obj_db == t_conn.end_obj_db && this.end_obj_entity == t_conn.end_obj_entity) || (this.start_obj_process == t_conn.end_obj_process && this.start_obj_db == t_conn.end_obj_db && this.start_obj_entity == t_conn.end_obj_entity && this.end_obj_process == t_conn.start_obj_process && this.end_obj_db == t_conn.start_obj_db && this.end_obj_entity == t_conn.start_obj_entity))
            {
                System.out.println("connection already exists");
                return false;
            }
        }
        
        return true;
    }
    
    public void get_connection()
    {
        double x1, x2, y1, y2;
        Point2D start_point, end_point;
        Line2D.Double temp_line;
        
        if(this.start_obj_process != null && this.end_obj_process != null)
        {
            x1 = start_obj_process.ell.x + start_obj_process.ell.width/2; 
            y1 = start_obj_process.ell.y + start_obj_process.ell.height/2;
            x2 = end_obj_process.ell.x + end_obj_process.ell.width/2;
            y2 = end_obj_process.ell.y + end_obj_process.ell.height/2;
            temp_line = new Line2D.Double(x1, y1, x2, y2);
            start_point = getIntPoint(start_obj_process.ell.x, start_obj_process.ell.y, start_obj_process.ell.width, start_obj_process.ell.height, temp_line);
            end_point = getIntPoint(end_obj_process.ell.x, end_obj_process.ell.y, end_obj_process.ell.width, end_obj_process.ell.height, temp_line);
        }
        
        else if(this.start_obj_process != null && this.end_obj_entity != null)
        {
            x1 = start_obj_process.ell.x + start_obj_process.ell.width/2; 
            y1 = start_obj_process.ell.y + start_obj_process.ell.height/2;
            x2 = end_obj_entity.rec.x + end_obj_entity.rec.width/2;
            y2 = end_obj_entity.rec.y + end_obj_entity.rec.height/2;
            temp_line = new Line2D.Double(x1, y1, x2, y2);
            start_point = getIntPoint(start_obj_process.ell.x, start_obj_process.ell.y, start_obj_process.ell.width, start_obj_process.ell.height, temp_line);
            end_point = getIntPoint(end_obj_entity.rec.x, end_obj_entity.rec.y, end_obj_entity.rec.width, end_obj_entity.rec.height, temp_line);
        }
        
        else if(this.start_obj_process != null && this.end_obj_db != null)
        {
            x1 = start_obj_process.ell.x + start_obj_process.ell.width/2; 
            y1 = start_obj_process.ell.y + start_obj_process.ell.height/2;
            x2 = end_obj_db.x + end_obj_db.width/2;
            y2 = end_obj_db.y + end_obj_db.height/2;
            temp_line = new Line2D.Double(x1, y1, x2, y2);
            start_point = getIntPoint(start_obj_process.ell.x, start_obj_process.ell.y, start_obj_process.ell.width, start_obj_process.ell.height, temp_line);
            end_point = getIntPoint(end_obj_db.x, end_obj_db.y, end_obj_db.width, end_obj_db.height, temp_line);
        }
        
        else if(this.start_obj_entity != null && this.end_obj_process != null)
        {
            x1 = start_obj_entity.rec.x + start_obj_entity.rec.width/2; 
            y1 = start_obj_entity.rec.y + start_obj_entity.rec.height/2;
            x2 = end_obj_process.ell.x + end_obj_process.ell.width/2;
            y2 = end_obj_process.ell.y + end_obj_process.ell.height/2;
            temp_line = new Line2D.Double(x1, y1, x2, y2);
            start_point = getIntPoint(start_obj_entity.rec.x, start_obj_entity.rec.y, start_obj_entity.rec.width, start_obj_entity.rec.height, temp_line);
            end_point = getIntPoint(end_obj_process.ell.x, end_obj_process.ell.y, end_obj_process.ell.width, end_obj_process.ell.height, temp_line);
        }
        
        else if(this.start_obj_entity != null && this.end_obj_entity != null)
        {
            x1 = start_obj_entity.rec.x + start_obj_entity.rec.width/2; 
            y1 = start_obj_entity.rec.y + start_obj_entity.rec.height/2;
            x2 = end_obj_entity.rec.x + end_obj_entity.rec.width/2;
            y2 = end_obj_entity.rec.y + end_obj_entity.rec.height/2;
            temp_line = new Line2D.Double(x1, y1, x2, y2);
            start_point = getIntPoint(start_obj_entity.rec.x, start_obj_entity.rec.y, start_obj_entity.rec.width, start_obj_entity.rec.height, temp_line);
            end_point = getIntPoint(end_obj_entity.rec.x, end_obj_entity.rec.y, end_obj_entity.rec.width, end_obj_entity.rec.height, temp_line);
        }
        
        else if(this.start_obj_entity != null && this.end_obj_db !=null)
        {
            x1 = start_obj_entity.rec.x + start_obj_entity.rec.width/2; 
            y1 = start_obj_entity.rec.y + start_obj_entity.rec.height/2;
            x2 = end_obj_db.x + end_obj_db.width/2;
            y2 = end_obj_db.y + end_obj_db.height/2;
            temp_line = new Line2D.Double(x1, y1, x2, y2);
            start_point = getIntPoint(start_obj_entity.rec.x, start_obj_entity.rec.y, start_obj_entity.rec.width, start_obj_entity.rec.height, temp_line);
            end_point = getIntPoint(end_obj_db.x, end_obj_db.y, end_obj_db.width, end_obj_db.height, temp_line);
        }
        
        else if(this.start_obj_db != null && this.end_obj_process != null)
        {
            x1 = start_obj_db.x + start_obj_db.width/2; 
            y1 = start_obj_db.y + start_obj_db.height/2;
            x2 = end_obj_process.ell.x + end_obj_process.ell.width/2;
            y2 = end_obj_process.ell.y + end_obj_process.ell.height/2;
            temp_line = new Line2D.Double(x1, y1, x2, y2);
            start_point = getIntPoint(start_obj_db.x, start_obj_db.y, start_obj_db.width, start_obj_db.height, temp_line);
            end_point = getIntPoint(end_obj_process.ell.x, end_obj_process.ell.y, end_obj_process.ell.width, end_obj_process.ell.height, temp_line);
        }
        
        else if(this.start_obj_db != null && this.end_obj_entity != null)
        {
            x1 = start_obj_db.x + start_obj_db.width/2; 
            y1 = start_obj_db.y + start_obj_db.height/2;
            x2 = end_obj_entity.rec.x + end_obj_entity.rec.width/2;
            y2 = end_obj_entity.rec.y + end_obj_entity.rec.height/2;
            temp_line = new Line2D.Double(x1, y1, x2, y2);
            start_point = getIntPoint(start_obj_db.x, start_obj_db.y, start_obj_db.width, start_obj_db.height, temp_line);
            end_point = getIntPoint(end_obj_entity.rec.x, end_obj_entity.rec.y, end_obj_entity.rec.width, end_obj_entity.rec.height, temp_line);
        }
        
        else
        {
            x1 = start_obj_db.x + start_obj_db.width/2; 
            y1 = start_obj_db.y + start_obj_db.height/2;
            x2 = end_obj_db.x + end_obj_db.width/2;
            y2 = end_obj_db.y + end_obj_db.height/2;
            temp_line = new Line2D.Double(x1, y1, x2, y2);
            start_point = getIntPoint(start_obj_db.x, start_obj_db.y, start_obj_db.width, start_obj_db.height, temp_line);
            end_point = getIntPoint(end_obj_db.x, end_obj_db.y, end_obj_db.width, end_obj_db.height, temp_line);
        }
        
        X1 = start_point.getX();
        Y1 = start_point.getY();
        X2 = end_point.getX();
        Y2 = end_point.getY();
        
    }
    
    public void draw_connection(Graphics2D g)
    {
        double mx, my;
        mx = (X1 + X2)/2; 
        my = (Y1 + Y2)/2;
        start.setLine(X1, Y1, mx, my);
        end.setLine(mx, my, X2, Y2);
        
        if(info_added_rtl)
            g.setColor(Color.BLUE);
        g.draw(start);
        
        if(info_added_ltr)
            g.setColor(Color.GREEN);
        else
            g.setColor(Color.RED);
        g.draw(end);
        
        g.setColor(Color.BLACK);
    }
    
    public Point2D getIntPoint(double xxx, double yyy, double www, double hhh, Line2D.Double lin)
    {
        Point2D inter_point = null;
        double min_dist = 1000000000;//change to diagnol of screen!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        double dist_range = Math.sqrt(Math.pow(www/2, 2) + Math.pow(hhh/2, 2));
        Rectangle2D.Double temp_rec = new Rectangle2D.Double(xxx, yyy, www, hhh);
        java.util.List<Point2D> intersections = new ArrayList<Point2D>();
        PathIterator it = temp_rec.getPathIterator(null);
        
        double[] coords = new double[6];
        double[] pos = new double[2];
        
        while (!it.isDone()) 
        {
            int type = it.currentSegment(coords);
            switch (type) 
            {
                
            case PathIterator.SEG_MOVETO:
            pos[0] = coords[0];
            pos[1] = coords[1];
            break;
                
            case PathIterator.SEG_LINETO:
            Line2D.Double l = new Line2D.Double(pos[0], pos[1], coords[0], coords[1]);
            pos[0] = coords[0];
            pos[1] = coords[1];
            Point2D intersection = getIntersection(lin, l);
            if (intersection != null)
                intersections.add(intersection);
            break;    
            }
            
            it.next();
        }
        for (Point2D s : intersections)
        {
            boolean xf = false;
            boolean yf = false;
            double xx = s.getX();
            double yy = s.getY();
            double dist = Math.sqrt(Math.pow(xx - (xxx + www/2), 2) + Math.pow(yy - (yyy + hhh/2), 2));
            
            if(lin.x1 > lin.x2)
            {
                if(xx < lin.x1 && xx > lin.x2)
                    xf = true;
            }
            
            else
            {
                if(xx > lin.x1 && xx < lin.x2)
                    xf = true;
            }
            
            if(lin.y1 > lin.y2)
            {
                if(yy < lin.y1 && yy > lin.y2)
                    yf = true;
            }
            
            else
            {
                if(yy > lin.y1 && yy < lin.y2)
                    yf = true;
            }
            
            if(xf && yf)//&& (dist <= dist_range + 5) && (dist >= dist_range - 5))
            {
                if(min_dist > dist)
                {
                    min_dist = dist;
                    inter_point = s;
                    System.out.println(xx + "==" + yy + "--inter point candidates--");
                }
            }
            
        }
        
        return inter_point;
    }
    
    public Point2D getIntersection(Line2D.Double line1, Line2D.Double line2)
    {
        double x1,y1, x2,y2, x3,y3, x4,y4;
        x1 = line1.x1; y1 = line1.y1; x2 = line1.x2; y2 = line1.y2;
        x3 = line2.x1; y3 = line2.y1; x4 = line2.x2; y4 = line2.y2;
        
        double x = (
                (x2 - x1)*(x3*y4 - x4*y3) - (x4 - x3)*(x1*y2 - x2*y1)
                ) /
                (
                (x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4)
                );
        
        double y = (
                (y3 - y4)*(x1*y2 - x2*y1) - (y1 - y2)*(x3*y4 - x4*y3)
                ) /
                (
                (x1 - x2)*(y3 - y4) - (y1 - y2)*(x3 - x4)
                );
        
        return new Point2D.Double(x, y);
    }
}

class database implements Serializable
{
    Line2D.Double upper_line;
    Line2D.Double lower_line;
    Line2D.Double right_line;
    Line2D.Double left_line;
    
    public String info;
    
    private static final long serialVersionUID = 7526472111622776140L;
    
    double x, y, width, height;
    
    public database(double xT, double yT, double w, double h)
    {
        this.upper_line = new Line2D.Double(xT, yT, xT + w, yT);
        this.lower_line = new Line2D.Double(xT, yT + h, xT + w, yT + h);
        this.right_line = new Line2D.Double(xT + w/10, yT, xT + w/10, yT + h);
        this.left_line = new Line2D.Double(xT, yT, xT, yT + h);
        this.info = "right click to change info";
        this.x = xT;
        this.y = yT;
        this.width = w;
        this.height = h;
    }
    
    public void resizeDatabase(double w, double h)
    {
        upper_line.setLine(x, y, x + w, y);
        lower_line.setLine(x, y + h, x + w, y + h);
        right_line.setLine(x + w/10, y, x + w/10, y + h);
        left_line.setLine(x, y, x, y + h);
        this.width = w;
        this.height = h;
    }
    
    public void drag_database(double xT, double yT)
    {
        this.x = xT;
        this.y = yT;
        upper_line.setLine(x, y, x + width, y);
        lower_line.setLine(x, y + height, x + width, y + height);
        right_line.setLine(x + width/10, y, x + width/10, y + height);
        left_line.setLine(x, y, x, y + height);
    }
    
    public void draw_database(Graphics2D g)
    {
        System.out.println("drawing..database");
        g.draw(upper_line);
        g.draw(lower_line);
        g.draw(left_line);
        g.draw(right_line);
    }
    
    public boolean mycontains(double xx, double yy)
    {
        if(xx < x + width && xx > x && yy < y + height && yy > y)
            return true;
        else
            return false;
    }
    
}

class entity implements Serializable
{ 
    Rectangle2D.Double rec;
    public String info;
    
    private static final long serialVersionUID = 7526472111622776147L;
    
    public entity(double xT, double yT, double w, double h)
    {
        this.info = "right click to change info";
        this.rec = new Rectangle2D.Double(xT, yT, w, h);
    }
    
    public void resizeEntity(double w, double h)
    {
        this.rec.setFrame(this.rec.x, this.rec.y, w, h);
    }
    
    public void drag_entity(double xT, double yT)
    {
        this.rec.x = xT;
        this.rec.y = yT;
    }
    
    public void draw_entity(Graphics2D g)
    {
        System.out.println("drawing..entity");
        g.draw(rec);
    }
    
}

class process implements Serializable
{
    Ellipse2D.Double ell;
    
    dfd_level next_level;
    boolean has_next_level;
    
    public String info;
    
    private static final long serialVersionUID = 7526472295622776147L;
    
    public process(double xT, double yT, double w, double h)
    {
        this.info = "right click to change info";
        this.has_next_level = false;
        this.ell = new Ellipse2D.Double(xT, yT, w, h);
    }
    
    public void resizeProcess(double w, double h)
    {
        this.ell.setFrame(this.ell.x, this.ell.y, w, h);
    }
    
    public void drag_process(double xT, double yT)
    {
        this.ell.x = xT;
        this.ell.y = yT;
    }
    
    public void draw_process(Graphics2D g)
    {
        System.out.println("drawing..process");
        g.draw(ell);
    }
    
    public dfd_level next_level(dfd_level curr_level)
    {
        if(!has_next_level)
        {
            this.next_level = new dfd_level(false, curr_level);
            this.has_next_level = true;
        }
        
        return this.next_level;
    }
    
}

class dfd_level implements Serializable
{
    private static final long serialVersionUID = 7521172295622776147L;
    
    Vector<process> list_of_procs = new Vector<process>(100,20);
    Vector<database> list_of_dbs = new Vector<database>(100,20);
    Vector<entity> list_of_entities = new Vector<entity>(100,20);
    Vector<connection> list_of_connections = new Vector<connection>(100,20);
    
    int process_count = -1;
    int entity_count = -1;
    int database_count = -1;
    int connection_count = -1;
    
    boolean start;
    dfd_level previous_level;
    
    public dfd_level(boolean startT, dfd_level previous)
    {
        this.start = startT;//if true then 0 level
        this.previous_level = previous; 
    }
    
    public void add_process(double xx, double yy)
    {
        if(start == true && process_count == 0)
            return;
        
        this.process_count++;
        this.list_of_procs.add(new process(xx, yy, 100, 100));
        System.out.println("process added");
        
        return;
    }
    
    public connection new_connection()
    {
        if((process_count == -1 && entity_count == -1) || (process_count == -1 && database_count == -1) || (database_count == -1 && entity_count == -1))
        {
            System.out.println("cannot provide a connection");
            return null;
        }
        
        System.out.println("connection provided");
        
        return new connection();
    }
    
    public void add_connection(connection temp_conn)
    {
        this.connection_count++;
        this.list_of_connections.add(temp_conn);
        System.out.println("connection added");
    }
    
    public void add_database(double xx, double yy)
    {
        if(start == true)
        {
            System.out.println("zero level cannot have databases");
            return;
        }
        
        this.database_count++;
        this.list_of_dbs.add(new database(xx, yy, 80, 20));
        System.out.println("database added");
        
        return;
    }
    
    public void add_entity(double xx, double yy)
    {
        this.entity_count++;
        this.list_of_entities.add(new entity(xx, yy, 80, 20));
        System.out.println("entity added");
        
        return;
    }
    
    public void delete_process(double xx, double yy)
    {
        process temp;
        Iterator<process> itr = list_of_procs.iterator();
        
        while(itr.hasNext())
        {
            temp = itr.next();
            
            if(temp.ell.contains(xx, yy))
            {
                itr.remove();
                break;
            }
        }
        
    }
    
    public void delete_connection(connection temp_conn)
    {
        this.list_of_connections.remove(temp_conn);
        this.connection_count--;
    }
    
    public void delete_entity(double xx, double yy)
    {
        entity temp;
        Iterator<entity> itr = list_of_entities.iterator();
        
        while(itr.hasNext())
        {
            temp = itr.next();
            
            if(temp.rec.contains(xx, yy))
            {
                itr.remove();
                break;
            }
        }
    }
    
    public void delete_database(double xx, double yy)
    {
        database temp;
        Iterator<database> itr = list_of_dbs.iterator();
        
        while(itr.hasNext())
        {
            temp = itr.next();
            
            if(temp.mycontains(xx, yy))
            {
                itr.remove();
                break;
            }
        }
    }
    
    public dfd_level prev_level()
    {
        return this.previous_level;
    }
    
    public boolean remove_connections_of_process(process temp_proc)
    {
        connection temp_connection;
        Iterator<connection> itr_connection = this.list_of_connections.iterator();
        
        while(itr_connection.hasNext())
        {
            temp_connection = itr_connection.next();
            
            if(temp_connection.start_obj_process == temp_proc || temp_connection.end_obj_process == temp_proc)
            {
                delete_connection(temp_connection);
                System.out.println("connection related to process was removed");
                
                return true;
            }
        }
        
        return false;
    }
    public boolean remove_connections_of_entity(entity temp_ent)
    {
        connection temp_connection;
        Iterator<connection> itr_connection = this.list_of_connections.iterator();
        
        while(itr_connection.hasNext())
        {
            temp_connection = itr_connection.next();
            
            if(temp_connection.start_obj_entity == temp_ent || temp_connection.end_obj_entity == temp_ent)
            {
                delete_connection(temp_connection);
                System.out.println("connection related to entity was removed");
                
                return true;
            }
        }
        
        return false;
    }
    public boolean remove_connections_of_db(database temp_db)
    {
        connection temp_connection;
        Iterator<connection> itr_connection = this.list_of_connections.iterator();
        
        while(itr_connection.hasNext())
        {
            temp_connection = itr_connection.next();
            
            if(temp_connection.start_obj_db == temp_db || temp_connection.end_obj_db == temp_db)
            {
                delete_connection(temp_connection);
                System.out.println("connection related to database was removed");
                
                return true;
            }
        }
        
        return false;
    }
}

public class seEditor implements ActionListener
{
    int XX, YY;
    process move_process;
    entity move_entity;
    database move_database;
    connection move_connection;
    String file_name;
    JFrame mainFrame;
    myJPanel drawingPanel;
    JMenuBar jm;
    int connection_select_count;
        
    JMenu jmFile;
    JMenuItem jmFileNew;
    JMenuItem jmFileOpen;
    JMenuItem jmFileSave;
    JMenuItem jmFileClose;
    JMenuItem jmFileExit;
    JMenuItem jmFileSaveAs;
    
    JMenu jmAdd;
    JMenuItem jmAddProc;
    JMenuItem jmAddEntity;
    JMenuItem jmAddConnect;
    JMenuItem jmAddDB;
    JMenuItem jmAddText;
    
    JMenu jmGoto;
    JMenuItem jmGotoZeroLevel;
    JMenuItem jmGotoPrevLevel;
    
    JTextField infoField;
    
    dfd_level current_level;
    
    obj_selected curr_obj;
    edit_states curr_state;
    
    public seEditor()
    {
        infoField = new JTextField();
        file_name="";
        curr_obj = obj_selected.NOTHING;
        curr_state = edit_states.IDLE;
        mainFrame = new JFrame("DFD Designer");
        drawingPanel = new myJPanel();
        mainFrame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        mainFrame.setLayout(new BorderLayout());
        mainFrame.add(drawingPanel, BorderLayout.CENTER);
        drawingPanel.setLayout(null);
        drawingPanel.setBorder(new LineBorder(Color.BLUE, 1, true));
        connection_select_count = 0;
        
        jm = new JMenuBar();
        
        jmFile = new JMenu("FILE");
        jmFileNew = new JMenuItem("New");
        jmFileOpen = new JMenuItem("Open");
        jmFileSave = new JMenuItem("Save");
        jmFileClose = new JMenuItem("Close");
        jmFileExit = new JMenuItem("Exit"); 
        jmFileSaveAs = new JMenuItem("Save as");
    
        jmAdd = new JMenu("ADD");
        jmAddProc = new JMenuItem("Process");
        jmAddEntity = new JMenuItem("Entity");
        jmAddConnect = new JMenuItem("Connection");
        jmAddDB = new JMenuItem("Database");
        jmAddText = new JMenuItem("Text");
        
        jmGoto = new JMenu("GOTO");
        jmGotoZeroLevel = new JMenuItem("Zero Level");
        jmGotoPrevLevel = new JMenuItem("Prev Level");
        
        jmFile.add(jmFileNew);
        jmFile.add(jmFileOpen);
        jmFile.add(jmFileSave);
        jmFile.add(jmFileSaveAs);
        jmFile.add(jmFileClose);
        jmFile.addSeparator();
        jmFile.add(jmFileExit);
        jm.add(jmFile);
        
        jmAdd.add(jmAddProc);
        jmAdd.add(jmAddEntity);
        jmAdd.add(jmAddDB);
        jmAdd.add(jmAddConnect);
        jmAdd.addSeparator();
        jmAdd.add(jmAddText);
        jm.add(jmAdd);
        
        jmGoto.add(jmGotoPrevLevel);
        jmGoto.add(jmGotoZeroLevel);
        jm.add(jmGoto);
        
        jm.add(infoField);
        
        infoField.addActionListener(this);
        jmFileNew.addActionListener(this);
        jmFileOpen.addActionListener(this);
        jmFileSave.addActionListener(this);
        jmFileSaveAs.addActionListener(this);
        jmFileClose.addActionListener(this);
        jmFileExit.addActionListener(this);
        jmAddProc.addActionListener(this);
        jmAddEntity.addActionListener(this);
        jmAddConnect.addActionListener(this);
        jmAddDB.addActionListener(this);
        jmAddText.addActionListener(this);
        jmGotoPrevLevel.addActionListener(this);
        jmGotoZeroLevel.addActionListener(this);
        
        mainFrame.setJMenuBar(jm);
        mainFrame.setSize(1366, 768);        
        
        mainFrame.setVisible(true);
    }
        
    
    class popup_menu implements ActionListener
    {
        String[] items;
        boolean success;
        int i;
        String show_str;
        int lORr_flag;
        public JPopupMenu popup;
        popup_menu()
        {
            popup = new JPopupMenu();
            JMenuItem item;
            popup.add(item = new JMenuItem("resize"));
            item.setHorizontalTextPosition(JMenuItem.RIGHT);
            item.addActionListener(this);
            popup.add(item = new JMenuItem("delete"));
            item.setHorizontalTextPosition(JMenuItem.RIGHT);
            item.addActionListener(this);
            popup.add(item = new JMenuItem("add info"));
            item.setHorizontalTextPosition(JMenuItem.RIGHT);
            item.addActionListener(this);
            popup.add(item = new JMenuItem("show info"));
            item.setHorizontalTextPosition(JMenuItem.RIGHT);
            item.addActionListener(this);
            popup.addSeparator();
            popup.add(item = new JMenuItem("info for connections"));
            item.addActionListener(this);
            popup.addSeparator();
            popup.setLabel("Justification");
            popup.setBorder(new BevelBorder(BevelBorder.RAISED));
            lORr_flag = 0;
            items = new String[2];
            items[0] = "";
            items[1] = "";
        }
        
        public void actionPerformed(ActionEvent ae)
        {
            if(curr_state == edit_states.SPAWN || curr_state == edit_states.MOVE)
            {
                JOptionPane.showMessageDialog(null, "object selected is: " + curr_obj + "\ncurrent operation is: " + curr_state + "\ncomplete the operation first");
                return;
            }
            if(current_level == null)
            {
                JOptionPane.showMessageDialog(null, "no file selected");
                return;
            }
            
            String val = ae.getActionCommand();
            
            process temp_process;
            Iterator<process> itr_process = current_level.list_of_procs.iterator();
            while(itr_process.hasNext())
            {
                temp_process = itr_process.next();
                if(temp_process.ell.contains(XX, YY))
                {
                    curr_obj = obj_selected.PROC;
                    move_process = temp_process;
                    break;
                }
            }
            
            entity temp_entity;
            Iterator<entity> itr_entity = current_level.list_of_entities.iterator();
            while(itr_entity.hasNext())
            {
                temp_entity = itr_entity.next();
                if(temp_entity.rec.contains(XX, YY))
                {
                    curr_obj = obj_selected.ENTITY;
                    move_entity = temp_entity;
                    break;
                }
            }
            
            database temp_database;
            Iterator<database> itr_database = current_level.list_of_dbs.iterator();
            while(itr_database.hasNext())
            {
                temp_database = itr_database.next();
                if(temp_database.mycontains(XX, YY))
                {
                    curr_obj = obj_selected.DB;
                    move_database = temp_database;
                    break;
                }
            }
            
            connection temp_connection;
            Iterator<connection> itr_connection = current_level.list_of_connections.iterator();
            while(itr_connection.hasNext())
            {
                temp_connection = itr_connection.next();
                if(temp_connection.mycontains(XX, YY))
                {
                    if(temp_connection.mycontains_ltr(XX, YY))
                        lORr_flag = 1;
                    if(temp_connection.mycontains_rtl(XX, YY))
                        lORr_flag = 2;
                    curr_obj = obj_selected.CONN;
                    move_connection = temp_connection;
                    break;
                }
            }
            
            System.out.println("==============" + curr_state + " =================== " + curr_obj + "==============");
            
            switch(val)
            {
                case "resize":
                
                if(curr_obj == obj_selected.PROC)
                {
                    curr_state = edit_states.RESIZE;
                    break;
                }
                
                if(curr_obj == obj_selected.ENTITY)
                {
                    curr_state = edit_states.RESIZE;
                    break;
                }
                
                if(curr_obj == obj_selected.DB)
                {
                    curr_state = edit_states.RESIZE;
                    break;
                }
                
                if(curr_obj == obj_selected.CONN)
                {
                    System.out.println("not valid for a connection object");
                    break;
                }
                
                case "delete":
                
                if(curr_obj == obj_selected.PROC)
                {
                    curr_state = edit_states.DIE;
                    break;
                }
                
                if(curr_obj == obj_selected.ENTITY)
                {
                    curr_state = edit_states.DIE;
                    break;
                }
                
                if(curr_obj == obj_selected.DB)
                {
                    //add are you sure joptionpane
                    curr_state = edit_states.DIE;
                    break;
                }
                
                if(curr_obj == obj_selected.CONN)
                {
                    curr_state = edit_states.DIE;
                    break;
                }
                
                case "add info":
                
                if(curr_obj == obj_selected.PROC)
                {
                    infoField.setText("enter process info here");
                    curr_state = edit_states.INFO;
                    break;
                }
                
                if(curr_obj == obj_selected.ENTITY)
                {
                    infoField.setText("enter entitiy info here");
                    curr_state = edit_states.INFO;
                    break;
                }
                
                if(curr_obj == obj_selected.DB)
                {
                    infoField.setText("enter database info here");
                    curr_state = edit_states.INFO;
                    break;
                }
                
                if(curr_obj == obj_selected.CONN)
                {
                    System.out.println("not valid for a connection object");
                    break;
                }
                
                case "show info":
                
                if(curr_obj == obj_selected.PROC)
                {
                    infoField.setText(move_process.info);
                    curr_obj = obj_selected.NOTHING;
                    break;
                }
                
                if(curr_obj == obj_selected.ENTITY)
                {
                    infoField.setText(move_entity.info);
                    curr_obj = obj_selected.NOTHING;
                    break;
                }
                
                if(curr_obj == obj_selected.DB)
                {
                    infoField.setText(move_database.info);
                    curr_obj = obj_selected.NOTHING;
                    break;
                }
                
                if(curr_obj == obj_selected.CONN)
                {
                    System.out.println("not valid for a connection object");
                    break;
                }
                
                case "info for connections":
                
                if(curr_obj == obj_selected.CONN)
                {
                    System.out.println("entered info for conneciton with flag value: " + lORr_flag);
                    String[] buttons = {"new", "edit", "delete", "show", "cancel"};
                    int rc = JOptionPane.showOptionDialog(null, "what do you wish to do?", "Confirmation", JOptionPane.YES_NO_CANCEL_OPTION, JOptionPane.QUESTION_MESSAGE,  null, buttons, buttons[4]);
                    
                    switch(rc)
                    {
                        case 0:
                        String temp_info = JOptionPane.showInputDialog("enter new info");
                        temp_info = temp_info.trim();
                        
                        if(temp_info == null)
                        {
                            System.out.println("no info added to connection");
                            break;
                        }
                        
                        else
                        {
                            if(lORr_flag == 1)
                            {
                                if(move_connection.info_ltr.isEmpty())
                                    move_connection.info_added_ltr = true;
                                move_connection.info_ltr.add(temp_info);
                            }
                            
                            else if(lORr_flag == 2)
                            {
                                if(move_connection.info_rtl.isEmpty())
                                    move_connection.info_added_rtl = true;
                                move_connection.info_rtl.add(temp_info);
                            }
                            
                            lORr_flag = 0;
                        }
                        break;
                        
                        case 1:
                        show_str = "";
                        success = true;
                        
                        if(lORr_flag == 1)
                        {
                            i = 1;
                            Iterator<String> itr = move_connection.info_ltr.iterator();
                            
                            while(itr.hasNext())
                            {
                                String temp_s = itr.next();
                                show_str = show_str + i++ + ": " + temp_s + "\n";
                            }
                            
                            show_str = show_str + "DON'T USE \':\' ANYWHERE EXCEPT AS THE INDEX VALUE SEPARATOR";
                            String info_index = JOptionPane.showInputDialog(show_str);
                            items = info_index.split(":");
                            
                            try
                            {
                                items[0] = items[0].trim();
                                i = Integer.parseInt(items[0]);
                            }
                            catch(NumberFormatException nfe)
                            {
                                System.out.println("invalid input");
                                success = false;
                            }
                            
                            if(success)
                            {
                                items[1] = items[1].trim();
                                if(move_connection.info_ltr.size() < i)
                                    System.out.println("invalid entry");
                                else
                                    move_connection.info_ltr.setElementAt(items[1], i-1);
                            }
                            
                        }
                        else if(lORr_flag == 2)
                        {
                            i = 1;
                            Iterator<String> itr = move_connection.info_rtl.iterator();
                            
                            while(itr.hasNext())
                            {
                                String temp_s = itr.next();
                                show_str = show_str + i++ + ": " + temp_s + "\n";
                            }
                            
                            show_str = show_str + "DON'T USE \':\' ANYWHERE EXCEPT AS THE INDEX VALUE SEPARATOR";
                            String info_index = JOptionPane.showInputDialog(show_str);
                            items = info_index.split(":");
                            
                            try
                            {
                                items[0] = items[0].trim();
                                i = Integer.parseInt(items[0]);
                            }
                            catch(NumberFormatException nfe)
                            {
                                System.out.println("invalid input");
                                success = false;
                            }
                            
                            if(success)
                            {
                                items[1] = items[1].trim();
                                if(move_connection.info_rtl.size() < i)
                                    System.out.println("invalid entry");
                                else
                                    move_connection.info_rtl.setElementAt(items[1], i-1);
                            }
                            
                        }
                        lORr_flag = 0;
                        break;
                        
                        case 2:
                        show_str = "";
                        success = true;
                        
                        if(lORr_flag == 1)
                        {
                            i = 1;
                            Iterator<String> itr = move_connection.info_ltr.iterator();
                            
                            while(itr.hasNext())
                            {
                                String temp_s = itr.next();
                                show_str = show_str + i++ + ": " + temp_s + "\n";
                            }
                            
                            show_str = show_str + "ENTER THE INDEX VALUE";
                            String info_index = JOptionPane.showInputDialog(show_str);
                            
                            try
                            {
                                info_index = info_index.trim();
                                i = Integer.parseInt(info_index);
                            }
                            catch(NumberFormatException nfe)
                            {
                                System.out.println("invalid input");
                                success = false;
                            }
                            
                            if(success)
                            {
                                if(move_connection.info_ltr.size() < i)
                                    System.out.println("invalid entry" + move_connection.info_ltr.size());
                            
                                else
                                {
                                    System.out.println("remove conn ind = " + i + "   " + lORr_flag);
                                    move_connection.info_ltr.remove(i-1);
                                }
                            }
                            
                            if(move_connection.info_ltr.isEmpty())
                                move_connection.info_added_ltr = false;
                        }
                        
                        else if(lORr_flag == 2)
                        {
                            i = 1;
                            Iterator<String> itr = move_connection.info_rtl.iterator();
                        
                            while(itr.hasNext())
                            {
                                String temp_s = itr.next();
                                show_str = show_str + i++ + ": " + temp_s + "\n";
                            }
                            
                            show_str = show_str + "ENTER THE INDEX VALUE";
                            String info_index = JOptionPane.showInputDialog(show_str);
                            
                            try
                            {
                                info_index = info_index.trim();
                                i = Integer.parseInt(info_index);
                            }
                            catch(NumberFormatException nfe)
                            {
                                System.out.println("invalid input");
                                success = false;
                            }
                            
                            if(success)
                            {
                                if(move_connection.info_rtl.size() < i)
                                    System.out.println("invalid entry" + move_connection.info_rtl.size());
                            
                                else
                                {
                                    System.out.println("remove conn ind = " + i + "   " + lORr_flag);
                                    move_connection.info_rtl.remove(i-1);
                                }
                            }
                            
                            if(move_connection.info_rtl.isEmpty())
                                move_connection.info_added_rtl = false;
                        
                        }
                        
                        lORr_flag = 0;
                        break;
                        
                        case 3:
                        show_str = "";
                        
                        if(lORr_flag == 1)
                        {
                            i = 1;
                            Iterator<String> itr = move_connection.info_ltr.iterator();
                        
                            while(itr.hasNext())
                            {
                                String temp_s = itr.next();
                                show_str = show_str + i++ + ": " + temp_s + "\n";
                            }
                            
                            JOptionPane.showMessageDialog(null, show_str);
                        }
                        
                        else if(lORr_flag == 2)
                        {
                            i = 1;
                            Iterator<String> itr = move_connection.info_rtl.iterator();
                        
                            while(itr.hasNext())
                            {
                                String temp_s = itr.next();
                                show_str = show_str + i++ + ": " + temp_s + "\n";
                            }
                            
                            JOptionPane.showMessageDialog(null, show_str);
                        }
                        
                        lORr_flag = 0;
                        break;
                        
                        case 4:
                        break;
                    }
                    
                    move_connection = null;
                    curr_obj = obj_selected.NOTHING;
                    drawingPanel.remote_paint();
                    break;
                }
                
                else
                {
                    System.out.println("not a valid option for this selected object");
                }
            }
        }
    }
    
    class myJPanel extends JPanel implements MouseListener, MouseMotionListener
    {
        popup_menu popup;
        
        private static final long serialVersionUID = 7526472295112776147L;
        
        public myJPanel()
        {
            addMouseListener(this);
            addMouseMotionListener(this);
            popup = new popup_menu();
        }
        
        public void mouseClicked(MouseEvent me)
        {
            boolean flag = false;
            
            if(current_level == null)
            {
                System.out.println("no file selected");
                return;
            }
            
            checkPopup(me);
            
            if(me.getClickCount() == 2 && curr_obj == obj_selected.NOTHING && curr_state == edit_states.IDLE)
            {
                
                process temp_process;
                Iterator<process> itr_process = current_level.list_of_procs.iterator();
                
                while(itr_process.hasNext())
                {
                    temp_process = itr_process.next();
                
                    if(temp_process.ell.contains(me.getX(), me.getY()))
                    {
                        move_process = temp_process;
                        break;
                    }
                }
                
                if(move_process != null)
                {
                    current_level = move_process.next_level(current_level);
                }
                
                else
                {
                    System.out.println("no next level");
                }
            
            }
            
            if(curr_obj == obj_selected.PROC && curr_state == edit_states.RESIZE)
            {
                double resize_val = ((me.getX() - move_process.ell.x) + (me.getY() - move_process.ell.y))/2;
                
                if(resize_val <= 10.0){}
            
                else
                {
                    move_process.resizeProcess(resize_val, resize_val);
                }
                
                move_process = null;
            }
            
            if(curr_obj == obj_selected.ENTITY && curr_state == edit_states.RESIZE)
            {
                double resize_val_x = me.getX() - move_entity.rec.x;
                double resize_val_y = me.getY() - move_entity.rec.y;
                
                if(resize_val_x <= 10.0 || resize_val_y <=10){}
                
                else
                    move_entity.resizeEntity(resize_val_x, resize_val_y);
                
                move_entity = null;
            }
            
            if(curr_obj == obj_selected.DB && curr_state == edit_states.RESIZE)
            {
                double resize_val_x = me.getX() - move_database.x;
                double resize_val_y = me.getY() - move_database.y;
                
                if(resize_val_x <= 10.0 || resize_val_y <=10){}
                
                else
                    move_database.resizeDatabase(resize_val_x, resize_val_y);
                
                move_database = null;
            }
            
            if(curr_obj == obj_selected.PROC && curr_state == edit_states.SPAWN)
            {
                System.out.println("Spawn process click state");
                current_level.add_process((double)me.getX(), (double)me.getY());
            }
            
            if(curr_obj == obj_selected.ENTITY && curr_state == edit_states.SPAWN)
            {
                entity temp;
                System.out.println("Spawn entity click state");
                current_level.add_entity((double)me.getX(), (double)me.getY());
            }
            
            if(curr_obj == obj_selected.DB && curr_state == edit_states.SPAWN)
            {
                database temp;
                System.out.println("Spawn database click state");
                current_level.add_database((double)me.getX(), (double)me.getY());
            }
            
            if(curr_obj == obj_selected.CONN && curr_state == edit_states.SPAWN)
            {
                boolean flag_conn = false;
            
                if(connection_select_count == 0)
                {
                    flag_conn = false;
                    process temp_process;
                    Iterator<process> itr_process = current_level.list_of_procs.iterator();
                    
                    while(itr_process.hasNext())
                    {
                        temp_process = itr_process.next();
                    
                        if(temp_process.ell.contains(me.getX(), me.getY()))
                        {
                            move_connection.select_start_obj_process(temp_process);  
                            System.out.println("start obj process");
                            flag_conn = true;
                        }
                    }
                    
                    if(!flag_conn)
                    {
                        System.out.println("enter select counter = " + connection_select_count + "entity");
                        entity temp_entity;
                        Iterator<entity> itr_entity = current_level.list_of_entities.iterator();
                    
                        while(itr_entity.hasNext())
                        {
                            temp_entity = itr_entity.next();
                        
                            if(temp_entity.rec.contains(me.getX(), me.getY()))
                            {
                                System.out.println("start obj entity");
                                move_connection.select_start_obj_entity(temp_entity);
                                flag_conn = true;
                            }
                        }
                    }
                    
                    if(!flag_conn)
                    {
                        System.out.println("enter select counter = " + connection_select_count + "database");
                        database temp_database;
                        Iterator<database> itr_database = current_level.list_of_dbs.iterator();
                        
                        while(itr_database.hasNext())
                        {
                            temp_database = itr_database.next();
                            
                            if(temp_database.mycontains(me.getX(), me.getY()))
                            {
                                System.out.println("start obj database");
                                move_connection.select_start_obj_database(temp_database);
                                flag_conn = true;
                            }
                        }
                    }
                    
                    if(flag_conn)
                    {
                        connection_select_count++;
                        System.out.println(" no of objects selected = " + connection_select_count);
                    }
                }
                
                else if(connection_select_count == 1)
                {
                    flag_conn = false;
                    process temp_process;
                    Iterator<process> itr_process = current_level.list_of_procs.iterator();
                    
                    while(itr_process.hasNext())
                    {
                        temp_process = itr_process.next();
                        
                        if(temp_process.ell.contains(me.getX(), me.getY()))
                        {
                            System.out.println("end obj process");
                            move_connection.select_end_obj_process(temp_process);  
                            flag_conn = true;
                        }
                    }
                    
                    if(!flag_conn)
                    {
                        entity temp_entity;
                        Iterator<entity> itr_entity = current_level.list_of_entities.iterator();
                        
                        while(itr_entity.hasNext())
                        {
                            temp_entity = itr_entity.next();
                            
                            if(temp_entity.rec.contains(me.getX(), me.getY()))
                            {
                                System.out.println("end obj entity");
                                move_connection.select_end_obj_entity(temp_entity);
                                flag_conn = true;
                            }
                        }
                    }
                    
                    if(!flag_conn)
                    {
                        database temp_database;
                        Iterator<database> itr_database = current_level.list_of_dbs.iterator();
                        
                        while(itr_database.hasNext())
                        {
                            temp_database = itr_database.next();
                            
                            if(temp_database.mycontains(me.getX(), me.getY()))
                            {
                                System.out.println("end obj database");
                                move_connection.select_end_obj_database(temp_database);
                                flag_conn = true;
                            }
                        }
                    }
                    
                    if(flag_conn)
                    {
                        connection_select_count++;
                        System.out.println(" no of objects selected = " + connection_select_count);
                    }
                }
                
                if(flag_conn && connection_select_count == 2)
                {
                    connection_select_count++;
                    System.out.println(" no of objects selected = " + connection_select_count);
                    flag_conn = move_connection.check_feasability();
                    
                    if(flag_conn && move_connection.check_feasability_2(current_level))
                    {
                        move_connection.get_connection();
                        current_level.connection_count++;
                        current_level.list_of_connections.add(move_connection);
                        System.out.println("connection added");
                    }
                    
                    else
                    {
                        System.out.println("connection discarded");
                    }
                    
                    move_connection = null;
                    connection_select_count = 0;
                }
            }
            
            if(curr_obj == obj_selected.PROC && curr_state == edit_states.DIE)
            {
                while(current_level.remove_connections_of_process(move_process));
                current_level.list_of_procs.remove(move_process);
                move_process = null;
                current_level.process_count--;
            }
            
            if(curr_obj == obj_selected.ENTITY && curr_state == edit_states.DIE)
            {
                while(current_level.remove_connections_of_entity(move_entity));
                current_level.list_of_entities.remove(move_entity);
                move_entity = null;
                current_level.entity_count--;
            }
            
            if(curr_obj == obj_selected.DB && curr_state == edit_states.DIE)
            {
                while(current_level.remove_connections_of_db(move_database));
                current_level.list_of_dbs.remove(move_database);
                move_database = null;
                current_level.database_count--;
            }
            
            if(curr_obj == obj_selected.CONN && curr_state == edit_states.DIE)
            {
                current_level.list_of_connections.remove(move_connection);
                move_connection = null;
                current_level.connection_count--;
            }
            
            if(connection_select_count == 0)
            {
                curr_obj = obj_selected.NOTHING;
                curr_state = edit_states.IDLE;
            }
            
            repaint();
        }
        
        public void draw_info(String info, Graphics g, double x, double y, double height, double width)
        {
            double ww = 0;
            int i = 0;
            double hh;
            double xx = x, yy = y, xxx;
            char c;
            double wt;
            double ht = height;
            
            g.setFont(new Font("", 0, 10));
            FontMetrics fm = getFontMetrics(new Font("", 0, 10));
            hh = fm.getLeading() + fm.getDescent() + fm.getAscent(); 
            
            ht = ht - hh - hh;
            yy = yy + hh + hh;
            
            while(ht > height/2)    
            {    
                wt = Math.sqrt((height*height)/4 - Math.pow((ht - (height/2)), 2));
                xx = x + height/2 - wt + 7;
                xxx = xx;
                wt = 2 * wt - 14;
                
                while(wt > 0)
                {
                    if(i == info.length() || info.length() == 0 )
                        return;
                    
                    c = info.charAt(i++);
                    ww = fm.charWidth(c);
                    
                    if(xx == xxx + wt && c == ' ')
                        continue;
                    
                    g.drawString(String.valueOf(c), (int)xx, (int)yy);
                    
                    xx = xx + ww;
                    wt = wt - ww;
                }
                
                ht = ht - hh;
                yy = yy + hh;
            }
            
            ht = ht - hh;
            
            while(ht > 0)    
            {
                wt = Math.sqrt((height*height)/4 - Math.pow((ht - (height/2)), 2));
                xx = x + height/2 - wt + 7;
                xxx = xx;
                wt = 2 * wt - 14;
            
                while(wt > 0)
                {
                    if(i == info.length() || info.length() == 0 )
                        return;
                    
                    c = info.charAt(i++);
                    ww = fm.charWidth(c);
                
                    if(xx == xxx + wt && c == ' ')
                        continue;
                    g.drawString(String.valueOf(c), (int)xx, (int)yy);
                    
                    xx = xx + ww;
                    wt = wt - ww;
                }
                
                ht = ht - hh;
                yy = yy + hh;
            }
        }
        
        public void draw_info_2(String info, Graphics g, double x, double y, double height, double width, double db)//flag=true then db
        {
            x = x + db+4;
            width = width - 7;
            
            double ww = 0;
            int i = 0;
            double hh;
            double xx, yy = y;
            char c;
            double wt;
            double ht = height;
            
            g.setFont(new Font("", 0, 10));
            FontMetrics fm = getFontMetrics(new Font("", 0, 10));
            hh = fm.getLeading() + fm.getDescent() + fm.getAscent(); 
            
            ht = ht - hh;
            yy = yy + hh;
            
            while(ht > 0)
            {
                xx = x;
                wt = width;
            
                while(wt > 0)
                {
                
                    if(i == info.length() || info.length() == 0 )
                        return;
                    
                
                    c = info.charAt(i++);
                
                    ww = fm.charWidth(c);
            
                    if(xx == x + wt && c == ' ')
                    continue;
                 
                    g.drawString(String.valueOf(c), (int)xx, (int)yy);
                
                    xx = xx + ww;
                    wt = wt - ww;
                
                }
                
                ht = ht - hh;
                yy = yy + hh;
            }
        }
    
        public void mouseEntered(MouseEvent me)
        {
        }
    
        public void mouseExited(MouseEvent me)
        {  
        }
    
        public void mousePressed(MouseEvent me)
        {
            checkPopup(me);
            
            if(current_level == null)
            {
                System.out.println("no file selected");
                return;
            }
            
            int mouseX = me.getX();
            int mouseY = me.getY();
            
            if(curr_state == edit_states.IDLE && curr_obj == obj_selected.NOTHING)
            {
                process temp_process;
                Iterator<process> itr_process = current_level.list_of_procs.iterator();
            
                while(itr_process.hasNext())
                {
                    temp_process = itr_process.next();
                    
                    if(temp_process.ell.contains(mouseX, mouseY))
                    {
                        curr_state = edit_states.MOVE;
                        curr_obj = obj_selected.PROC;
                        move_process = temp_process;
                        break;
                    }
                }
                
                entity temp_entity;
                Iterator<entity> itr_entity = current_level.list_of_entities.iterator();
                
                while(itr_entity.hasNext())
                {
                    temp_entity = itr_entity.next();
                    
                    if(temp_entity.rec.contains(mouseX, mouseY))
                    {
                        curr_state = edit_states.MOVE;
                        curr_obj = obj_selected.ENTITY;
                        move_entity = temp_entity;
                        break;
                    }
                }
                
                database temp_database;
                Iterator<database> itr_database = current_level.list_of_dbs.iterator();
                
                while(itr_database.hasNext())
                {
                    temp_database = itr_database.next();
                    
                    if(temp_database.mycontains(mouseX, mouseY))
                    {
                        curr_state = edit_states.MOVE;
                        curr_obj = obj_selected.DB;
                        move_database = temp_database;
                        break;
                    }
                }
            }
        }
        
        public void remote_paint()
        {
            repaint();
        }
    
        public void mouseReleased(MouseEvent me)
        {
            checkPopup(me);
            
            if(current_level == null)
            {
                System.out.println("no file selected");
                return;
            }
            
            if(curr_state == edit_states.MOVE && curr_obj == obj_selected.PROC)
            {
                repaint();
                move_process = null;
                curr_state = edit_states.IDLE;
                curr_obj = obj_selected.NOTHING;
            }
            
            if(curr_state == edit_states.MOVE && curr_obj == obj_selected.ENTITY)
            {
                repaint();
                move_entity = null;
                curr_state = edit_states.IDLE;
                curr_obj = obj_selected.NOTHING;
            }
            
            if(curr_state == edit_states.MOVE && curr_obj == obj_selected.DB)
            {
                repaint();
                move_database = null;
                curr_state = edit_states.IDLE;
                curr_obj = obj_selected.NOTHING;
            }
            
        }    
    
        public void mouseDragged(MouseEvent me)
        {
            if(current_level == null)
            {
                System.out.println("no file selected");
                return;
            }
            
            int mouseX = me.getX();
            int mouseY = me.getY();
            
            if(curr_state == edit_states.MOVE && curr_obj == obj_selected.PROC)
            {
                move_process.drag_process(mouseX, mouseY);
                repaint();
            }
            
            if(curr_state == edit_states.MOVE && curr_obj == obj_selected.ENTITY)
            {
                move_entity.drag_entity(mouseX, mouseY);
                repaint();
            }
            
            if(curr_state == edit_states.MOVE && curr_obj == obj_selected.DB)
            {
                move_database.drag_database(mouseX, mouseY);
                repaint();
            }
        }     
    
        public void mouseMoved(MouseEvent me)
        {
            if(curr_state == edit_states.IDLE && curr_obj == obj_selected.NOTHING)
                infoField.setText(" " + me.getX() + " " + me.getY() + " ");
        }
        
        void checkPopup(MouseEvent me) 
        {
            if(me.isPopupTrigger()) 
            {
                XX = me.getX();
                YY = me.getY();
                popup.popup.show(myJPanel.this, XX, YY);
            }
        }
    
        public void paintComponent(Graphics g)
        {
            super.paintComponent(g);
            
            if(current_level == null)
                return;
            
            Iterator<process> itr_process = current_level.list_of_procs.iterator();
            process temp_process;
            
            while(itr_process.hasNext())
            {
                temp_process = itr_process.next();
                System.out.println("process painted at: "+temp_process.ell.x + ", " + temp_process.ell.y + ", " + temp_process.ell.width + ", " + temp_process.ell.height + " " + "process count " + current_level.process_count + " info: " + temp_process.info);
                temp_process.draw_process((Graphics2D)g);
                draw_info(temp_process.info, g, temp_process.ell.x, temp_process.ell.y, temp_process.ell.height, temp_process.ell.width);
            }
            
            Iterator<entity> itr_entity = current_level.list_of_entities.iterator();
            entity temp_entity;
            
            while(itr_entity.hasNext())
            {
                temp_entity = itr_entity.next();
                System.out.println("entity painted at: "+temp_entity.rec.x + ", " + temp_entity.rec.y + ", " + temp_entity.rec.width + ", " + temp_entity.rec.height + " " + "entity count " + current_level.entity_count + " info: " + temp_entity.info);
                temp_entity.draw_entity((Graphics2D)g);
                draw_info_2(temp_entity.info, g, temp_entity.rec.x, temp_entity.rec.y, temp_entity.rec.height, temp_entity.rec.width, 0);
            }
            
            Iterator<database> itr_database = current_level.list_of_dbs.iterator();
            database temp_database;
            
            while(itr_database.hasNext())
            {
                temp_database = itr_database.next();
                System.out.println("entity painted at: "+temp_database.x + ", " + temp_database.y + ", " + temp_database.width + ", " + temp_database.height + " " + "database count: " + current_level.database_count + " info:" + temp_database.info);
                temp_database.draw_database((Graphics2D)g);
                draw_info_2(temp_database.info, g, temp_database.x, temp_database.y, temp_database.height, temp_database.width, temp_database.width/10);
            }
            
            Iterator<connection> itr_connection = current_level.list_of_connections.iterator();
            connection temp_connection;
            
            while(itr_connection.hasNext())
            {
                temp_connection = itr_connection.next();
                System.out.println("connection painted with connection count: " + current_level.connection_count);
                temp_connection.get_connection();
                temp_connection.draw_connection((Graphics2D)g);
            }
        }    
    }
    
    public void actionPerformed(ActionEvent ae)
    {
        if(curr_state == edit_states.INFO)
        {
            System.out.print("Writing info inside..");
            
            if(curr_obj == obj_selected.PROC)
            {
                System.out.println("process");
                move_process.info = infoField.getText();
                move_process = null;
            }
            
            if(curr_obj == obj_selected.ENTITY)
            {
                System.out.println("entity");
                move_entity.info = infoField.getText();
                move_entity = null;
            }
            
            if(curr_obj == obj_selected.DB)
            {
                System.out.println("database");
                move_database.info = infoField.getText();
                move_database = null;
            }
            
            curr_obj = obj_selected.NOTHING;
            curr_state = edit_states.IDLE;
            drawingPanel.remote_paint();
        }
        
        if(curr_state != edit_states.IDLE && curr_obj != obj_selected.NOTHING)
        {
            JOptionPane.showMessageDialog(null, "object selected is: " + curr_obj + "\ncurrent operation is: " + curr_state + "\ncomplete the operation first");
            return;
        }
        
        boolean success = true;
        
        String value = ae.getActionCommand();
        
        switch(value)
        {
            /*-------------------------------------------------*/
            case "New":
            if(current_level != null)
            {
                int res = JOptionPane.showConfirmDialog((Component) null, "discard unsaved progress?","alert", JOptionPane.YES_NO_OPTION);
                if(res == 1)
                    break;
            }
            file_name = JOptionPane.showInputDialog("enter file name");
            if(file_name == null)
                break;
            current_level = new dfd_level(true, null);
            //check for saving the current file or discarding
            success = true;
            break;
            
            /*-------------------------------------------------*/
            case "Process": 
            if(current_level == null)
            {
                JOptionPane.showMessageDialog(null, "no file selected");
                System.out.println("no file selected");
                break;
            }
            curr_obj = obj_selected.PROC;
            curr_state = edit_states.SPAWN;
            System.out.println("spawn process");
            break;              
            /*-------------------------------------------------*/
            case "Entity":
            if(current_level == null)
            {
                JOptionPane.showMessageDialog(null, "no file selected");
                System.out.println("no file selected");
                break;
            }
            curr_obj = obj_selected.ENTITY;
            curr_state = edit_states.SPAWN;
            System.out.println("spawn entity");
            break;              
            /*-------------------------------------------------*/
            case "Database":
            if(current_level == null)
            {
                JOptionPane.showMessageDialog(null, "no file selected");
                System.out.println("no file selected");
                break;
            }
            curr_obj = obj_selected.DB;
            curr_state = edit_states.SPAWN;
            System.out.println("spawn database");
            break;              
            /*-------------------------------------------------*/
            case "Connection":
            if(current_level == null)
            {
                JOptionPane.showMessageDialog(null, "no file selected");
                System.out.println("no file selected");
                break;
            }
            curr_obj = obj_selected.CONN;
            curr_state = edit_states.SPAWN;
            System.out.println("receive new connection");
            move_connection = current_level.new_connection();
            if(move_connection == null)
            {
                curr_obj = obj_selected.NOTHING;
                curr_state = edit_states.IDLE;
            }
            break;              
            /*-------------------------------------------------*/
            case "Save":
            if(current_level == null)
            {
                JOptionPane.showMessageDialog(null, "no file selected");
                System.out.println("no file selected");
                break;
            }
            //add do you want to continue this 
            System.out.println("entered save case");
            dfd_level temp_current_level = current_level;
            while(temp_current_level.prev_level() != null)
            {
                temp_current_level = temp_current_level.prev_level();
            }
            try
            {
                FileOutputStream fileOut = new FileOutputStream(file_name + ".ser");
                ObjectOutputStream out = new ObjectOutputStream(fileOut);
                out.writeObject(temp_current_level);
                out.close();
                fileOut.close();
            }
            catch(IOException e)
            {
                e.printStackTrace();
                success = false;
                System.out.println("---------------------------------unsuccessful-------------------------------open");
            }
            if(success)
            {
                System.out.println("-----------------------------------successful-------------------------------save");
            }
            else
            {
                success = true;
            }
            break; 
            /*-------------------------------------------------*/
            case "Save as":
            if(current_level == null)
            {
                JOptionPane.showMessageDialog(null, "no file selected");
                System.out.println("no file selected");
                break;
            }
            //add do you want to continue this 
            System.out.println("entered saveas case");
            dfd_level temp_current_level_as = current_level;
            while(temp_current_level_as.prev_level() != null)
            {
                temp_current_level_as = temp_current_level_as.prev_level();
            }
            file_name = JOptionPane.showInputDialog("enter file name");
            if(file_name == null)
                break;
            try
            {
                FileOutputStream fileOut = new FileOutputStream(file_name + ".ser");
                ObjectOutputStream out = new ObjectOutputStream(fileOut);
                out.writeObject(temp_current_level_as);
                out.close();
                fileOut.close();
            }
            catch(IOException e)
            {
                e.printStackTrace();
                success = false;
                System.out.println("---------------------------------unsuccessful-------------------------------open");
            }
            if(success)
            {
                System.out.println("-----------------------------------successful-------------------------------save");
            }
            else
            {
                success = true;
            }
            break; 
            /*-------------------------------------------------*/
            case "Open":
            System.out.println("entered open case");
            if(current_level != null)
            {
                int res = JOptionPane.showConfirmDialog((Component) null, "discard unsaved progress?","alert", JOptionPane.YES_NO_OPTION);
                if(res == 1)
                    break;
            }
            file_name = JOptionPane.showInputDialog("enter file name");
            //add do you want to save existing file yes no cancel
            try
            {
                FileInputStream fileIn = new FileInputStream(file_name  + ".ser");
                ObjectInputStream in = new ObjectInputStream(fileIn);
                current_level = (dfd_level) in.readObject();
                in.close();
                fileIn.close();
            }
            catch(FileNotFoundException e)
            {
                JOptionPane.showMessageDialog(null, "file not found");
            }
            catch(IOException e)
            {
                e.printStackTrace();
                success = false;
            }
            catch(ClassNotFoundException e)
            {
                e.printStackTrace();
                success = false;
            }
            if(success)
            {
                System.out.println("-----------------------------------successful-------------------------------open");
            }
            else
            {
                success = true;
                System.out.println("---------------------------------unsuccessful-------------------------------open");
            }
            drawingPanel.remote_paint();
            break;   
            /*-------------------------------------------------*/
            case "Close":
            if(current_level != null)
            {
                int res = JOptionPane.showConfirmDialog((Component) null, "discard unsaved progress?","alert", JOptionPane.YES_NO_OPTION);
                if(res == 1)
                  break;
            }
            current_level = null;
            drawingPanel.remote_paint();
            break;
            /*-------------------------------------------------*/
            case "Exit":
            if(current_level != null)
            {
                int res = JOptionPane.showConfirmDialog((Component) null, "discard unsaved progress?","alert", JOptionPane.YES_NO_OPTION);
                if(res == 1)
                  break;
            }
            System.exit(0);
            break;
            /*-------------------------------------------------*/
            case "Prev Level":
            if(current_level.prev_level() != null)
                current_level = current_level.prev_level();
            drawingPanel.remote_paint();
            break;
            /*-------------------------------------------------*/
            case "Zero Level":
            while(current_level.prev_level() != null)
                current_level = current_level.prev_level();
            drawingPanel.remote_paint();
            /*-------------------------------------------------*/
        }
    }
            
    public static void main(String args[])
    {
        SwingUtilities.invokeLater(new Runnable()
                                   {
                                       public void run()
                                       {
                                           new seEditor();
                                       }
                                   }
                                  );
    }
}
