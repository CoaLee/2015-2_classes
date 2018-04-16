package ds.powergame;

public class PowerGame {
	private ParPtrTree PPT;
	private Team[] TeamList;
	private static final int MAXPLAYER = 100000;
	
	public PowerGame(){
		PPT = new ParPtrTree(MAXPLAYER);
		TeamList = new Team[MAXPLAYER];
	}
	
	public boolean Merge(int player1, int player2){
		int leader1 = getRootLeader(player1);
		int leader2 = getRootLeader(player2);
		int power1 = TeamList[leader1].GetTeamPower();
		int power2 = TeamList[leader2].GetTeamPower();
		
		if(power1==power2) return false;
		else if(power1>power2) {
			PPT.UNION(player1, player2);
			TeamList[leader2].setLeader(leader1);
			TeamList[leader1].addTeamPower(TeamList[leader2].GetTeamPower());
		} else {
			PPT.UNION(player2, player1);
			TeamList[leader1].setLeader(leader2);
			TeamList[leader2].addTeamPower(TeamList[leader1].GetTeamPower());
		}
		return true;
	}
	
	public void Login(int player, int power){
		TeamList[player]=new Team(player, power);
		return;
	}
	
	public void PrintLeader(int player){
		System.out.println("LEADER: "+ getRootLeader(player));
		return;
	}
	
	public void PrintPower(int player){
		System.out.println("POWER: " + TeamList[getRootLeader(player)].GetTeamPower());
		return;
	}
	
	private int getRootLeader(int player){
		int tmp=player;
		if(TeamList[tmp].GetLeader()!=tmp)
			TeamList[tmp].setLeader(getRootLeader(TeamList[tmp].GetLeader()));
		return TeamList[tmp].GetLeader();
	}
}
